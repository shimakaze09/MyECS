#include "Archetype.hpp"

using namespace My::MyECS;
using namespace std;

Archetype::Archetype(std::pmr::memory_resource* rsrc,
                     std::pmr::synchronized_pool_resource* sync_rsrc,
                     synchronized_monotonic_buffer_resource* sync_frame_rsrc,
                     std::uint64_t version) noexcept
    : chunkAllocator{rsrc},
      version{version},
      sync_rsrc{sync_rsrc},
      sync_frame_rsrc{sync_frame_rsrc} {}

Archetype::~Archetype() {
  if (cmptTraits.IsTrivial()) return;

  for (std::size_t i = 0; i < cmptTraits.GetTraits().size(); i++) {
    const auto& trait = cmptTraits.GetTraits()[i];
    if (!trait.dtor) continue;
    for (std::size_t k = 0; k < chunks.size(); k++) {
      std::size_t num = chunks[k]->EntityNum();
      std::uint8_t* buffer = chunks[k]->data;
      std::uint8_t* beg = buffer + offsets[i];
      for (std::size_t i = 0; i < num; i++) {
        std::uint8_t* address = beg + i * trait.size;
        trait.dtor(address);
      }
    }
  }
  // FastClear by EntityMngr
  // for (Chunk* chunk : chunks)
  //	entityMngr->sharedChunkPool.Recycle(chunk);
}

Archetype::Archetype(std::pmr::memory_resource* rsrc,
                     std::pmr::synchronized_pool_resource* sync_rsrc,
                     synchronized_monotonic_buffer_resource* sync_frame_rsrc,
                     const Archetype& src)
    : chunkAllocator{rsrc},
      sync_rsrc{sync_rsrc},
      sync_frame_rsrc{sync_frame_rsrc} {
  cmptTraits = src.cmptTraits;
  chunkCapacity = src.chunkCapacity;
  offsets = src.offsets;
  version = src.version;
  entityNum = src.entityNum;
  nonFullChunks = src.nonFullChunks;

  chunks.resize(src.chunks.size(), nullptr);

  if (cmptTraits.IsTrivial()) {
    for (std::size_t i = 0; i < src.chunks.size(); i++) {
      chunks[i] = chunkAllocator.allocate(1);
      auto* srcChunk = src.chunks[i];
      auto* dstChunk = chunks[i];
      std::memcpy(dstChunk, srcChunk,
                  sizeof(Chunk::Head) + sizeof(Chunk::Head::CmptInfo) *
                                            cmptTraits.GetTypes().size());
      new (&dstChunk->GetHead()->chunk_unsync_rsrc)
          std::pmr::unsynchronized_pool_resource(sync_rsrc);
      new (&dstChunk->GetHead()->chunk_unsync_frame_rsrc)
          std::pmr::monotonic_buffer_resource(sync_frame_rsrc);
      dstChunk->GetHead()->ForceUpdateVersion(version);
      dstChunk->GetHead()->archetype = this;
      std::size_t num = srcChunk->EntityNum();
      for (std::size_t j = 0; j < cmptTraits.GetTraits().size(); j++) {
        const auto& trait = cmptTraits.GetTraits()[j];
        auto* srcBegin = srcChunk->data + offsets[j];
        auto* dstBegin = dstChunk->data + offsets[j];
        memcpy(dstBegin, srcBegin, num * trait.size);
      }
    }
  } else {
    for (std::size_t i = 0; i < src.chunks.size(); i++) {
      auto* srcChunk = src.chunks[i];
      auto* dstChunk = chunks[i] = chunkAllocator.allocate(1);
      std::memcpy(dstChunk, srcChunk,
                  sizeof(Chunk::Head) + sizeof(Chunk::Head::CmptInfo) *
                                            cmptTraits.GetTypes().size());
      new (&dstChunk->GetHead()->chunk_unsync_rsrc)
          std::pmr::unsynchronized_pool_resource(sync_rsrc);
      new (&dstChunk->GetHead()->chunk_unsync_frame_rsrc)
          std::pmr::monotonic_buffer_resource(sync_frame_rsrc);
      dstChunk->GetHead()->ForceUpdateVersion(version);
      dstChunk->GetHead()->archetype = this;
      std::size_t num = srcChunk->EntityNum();
      for (std::size_t j = 0; j < cmptTraits.GetTraits().size(); j++) {
        const auto& trait = cmptTraits.GetTraits()[j];
        auto* cursor_src = srcChunk->data + offsets[j];
        auto* cursor_dst = dstChunk->data + offsets[j];
        if (trait.copy_ctor) {
          for (std::size_t k = 0; k < num; k++) {
            trait.copy_ctor(cursor_dst, cursor_src,
                            dstChunk->GetChunkUnsyncResource());
            cursor_src += trait.size;
            cursor_dst += trait.size;
          }
        } else
          memcpy(cursor_dst, cursor_src, num * trait.size);
      }
    }
  }
}

void Archetype::SetLayout() {
  struct Item {
    std::size_t alignment;
    std::size_t idx;
  };

  const auto& traits = cmptTraits.GetTraits();
  small_vector<Item> items;
  items.resize(traits.size());

  std::size_t sum_size = 0;
  for (std::size_t i = 0; i < items.size(); i++) {
    items[i].idx = i;
    const auto& trait = cmptTraits.GetTraits()[i];
    items[i].alignment = trait.alignment;
    sum_size += trait.size;
  }
  std::sort(items.begin(), items.end(), [](const Item& lhs, const Item& rhs) {
    return lhs.alignment > rhs.alignment;
  });
  sum_size =
      (sum_size + items.front().alignment - 1) & ~(items.front().alignment - 1);

  std::size_t cmptInfosSize =
      sizeof(Chunk::Head::CmptInfo) * cmptTraits.GetTypes().size();
  chunkCapacity = (ChunkSize - sizeof(Chunk::Head) - cmptInfosSize) / sum_size;
  offsets.resize(traits.size());
  std::size_t offset = sizeof(Chunk::Head) + cmptInfosSize;
  offset =
      (offset + items.front().alignment - 1) & ~(items.front().alignment - 1);
  for (std::size_t i = 0; i < items.size(); i++) {
    const auto& idx = items[i].idx;
    offsets[idx] = offset;
    offset += chunkCapacity * traits[idx].size;
  }
}

Archetype* Archetype::New(
    CmptTraits& rtdCmptTraits, std::pmr::memory_resource* rsrc,
    std::pmr::synchronized_pool_resource* sync_rsrc,
    synchronized_monotonic_buffer_resource* sync_frame_rsrc,
    std::span<const TypeID> types, std::uint64_t version) {
  assert(std::find(types.begin(), types.end(), TypeID_of<Entity>) ==
         types.end());

  auto* rst = new Archetype{rsrc, sync_rsrc, sync_frame_rsrc, version};

  rst->cmptTraits.Register(rtdCmptTraits, TypeID_of<Entity>);
  for (const auto& type : types) rst->cmptTraits.Register(rtdCmptTraits, type);

  rst->SetLayout();

  return rst;
}

Archetype* Archetype::Add(CmptTraits& rtdCmptTraits, const Archetype* from,
                          std::span<const TypeID> types) {
  assert(std::find(types.begin(), types.end(), TypeID_of<Entity>) ==
         types.end());
  assert(std::find_if_not(types.begin(), types.end(), [&](const auto& type) {
           return from->cmptTraits.GetTypes().contains(type);
         }) != types.end());

  auto* rst = new Archetype{from->chunkAllocator.resource(), from->sync_rsrc,
                            from->sync_frame_rsrc, from->version};

  rst->cmptTraits = from->cmptTraits;
  for (const auto& type : types) rst->cmptTraits.Register(rtdCmptTraits, type);

  rst->SetLayout();

  return rst;
}

Archetype* Archetype::Remove(const Archetype* from,
                             std::span<const TypeID> types) {
  assert(std::find(types.begin(), types.end(), TypeID_of<Entity>) ==
         types.end());
  assert(std::find_if(types.begin(), types.end(), [&](const auto& type) {
           return from->cmptTraits.GetTypes().contains(type);
         }) != types.end());

  auto* rst = new Archetype{from->chunkAllocator.resource(), from->sync_rsrc,
                            from->sync_frame_rsrc, from->version};

  rst->cmptTraits = from->cmptTraits;

  for (const auto& type : types) rst->cmptTraits.Deregister(type);

  rst->SetLayout();

  return rst;
}

Archetype::EntityAddress Archetype::Create(Entity e) {
  EntityAddress addr = RequestBuffer();
  std::size_t idxInChunk = addr.idxInChunk;
  Chunk* chunk = chunks[addr.chunkIdx];
  std::uint8_t* buffer = chunk->data;

  for (std::size_t i = 0; i < cmptTraits.GetTraits().size(); i++) {
    const auto& trait = cmptTraits.GetTraits()[i];
    const std::size_t offset = offsets[i];
    const auto& type = *(cmptTraits.GetTypes().begin() + i);
    if (type.Is<Entity>()) {
      constexpr std::size_t size = sizeof(Entity);
      memcpy(buffer + offset + idxInChunk * size, &e, size);
    } else {
      std::uint8_t* dst = buffer + offset + idxInChunk * trait.size;
      trait.DefaultConstruct(dst, chunk->GetChunkUnsyncResource());
    }
  }
  chunk->GetHead()->ForceUpdateVersion(version);

  entityNum++;

  return addr;
}

Archetype::EntityAddress Archetype::RequestBuffer() {
  if (nonFullChunks.empty()) {
    auto* chunk = chunkAllocator.allocate(1);

    // init chunk
    chunk->GetHead()->archetype = this;
    chunk->GetHead()->capacity = chunkCapacity;
    chunk->GetHead()->num_entity = 0;
    chunk->GetHead()->num_component = cmptTraits.GetTypes().size();
    for (std::size_t i = 0; i < cmptTraits.GetTypes().size(); i++) {
      auto& info = chunk->GetHead()->GetCmptInfos()[i];
      info.ID = cmptTraits.GetTypes().data()[i];
      info.offset = offsets[i];
    }
    chunk->GetHead()->ForceUpdateVersion(version);
    new (&chunk->GetHead()->chunk_unsync_rsrc)
        std::pmr::unsynchronized_pool_resource(sync_rsrc);
    new (&chunk->GetHead()->chunk_unsync_frame_rsrc)
        std::pmr::monotonic_buffer_resource(sync_frame_rsrc);

    chunks.push_back(chunk);

    nonFullChunks.insert(chunks.size() - 1);
  }

  std::size_t chunkIdx = nonFullChunks.back();
  Chunk::Head* chunkhead = chunks[chunkIdx]->GetHead();
  std::size_t idxInChunk = chunkhead->num_entity++;

  if (chunkhead->num_entity == chunkhead->capacity)
    nonFullChunks.erase(chunkIdx);

  return EntityAddress{.chunkIdx = chunkIdx, .idxInChunk = idxInChunk};
}

CmptAccessPtr Archetype::At(AccessTypeID type, EntityAddress addr) const {
  auto target = cmptTraits.GetTypes().find(type);
  if (target == cmptTraits.GetTypes().end()) return nullptr;
  auto typeIdx = static_cast<std::size_t>(
      std::distance(cmptTraits.GetTypes().begin(), target));

  const auto& trait = cmptTraits.GetTraits()[typeIdx];
  std::size_t size = trait.size;
  std::size_t offset = offsets[typeIdx];
  std::size_t idxInChunk = addr.idxInChunk;
  Chunk* chunk = chunks[addr.chunkIdx];
  std::uint8_t* buffer = chunk->data;
  if (type.GetAccessMode() == AccessMode::WRITE)
    chunk->GetHead()->GetCmptInfos()[typeIdx].version = version;

  return {type, buffer + offset + idxInChunk * size};
}

Archetype::EntityAddress Archetype::Instantiate(Entity e, EntityAddress src) {
  EntityAddress dst;

  Chunk* srcChunk = chunks[src.chunkIdx];
  if (!srcChunk->Full()) {
    dst = {.chunkIdx = src.chunkIdx,
           .idxInChunk = srcChunk->GetHead()->num_entity++};
    if (srcChunk->Full()) nonFullChunks.erase(src.chunkIdx);
  } else
    dst = RequestBuffer();

  std::size_t srcIdxInChunk = src.idxInChunk;
  std::size_t dstIdxInChunk = dst.idxInChunk;
  std::uint8_t* srcBuffer = srcChunk->data;
  Chunk* dstChunk = chunks[dst.chunkIdx];
  std::uint8_t* dstBuffer = dstChunk->data;

  for (std::size_t i = 0; i < cmptTraits.GetTypes().size(); i++) {
    const auto& trait = cmptTraits.GetTraits()[i];
    std::size_t offset = offsets[i];

    std::size_t size = trait.size;
    std::uint8_t* dst = dstBuffer + offset + dstIdxInChunk * size;
    std::uint8_t* src = srcBuffer + offset + srcIdxInChunk * size;

    trait.CopyConstruct(dst, src, dstChunk->GetChunkUnsyncResource());
  }

  dstChunk->GetHead()->ForceUpdateVersion(version);

  entityNum++;

  return dst;
}

std::tuple<My::small_vector<Entity*>,
           My::small_vector<My::small_vector<CmptAccessPtr>>,
           My::small_vector<std::size_t>>
Archetype::Locate(std::span<const AccessTypeID> cmpts) const {
  assert(
      std::find_if_not(cmpts.begin(), cmpts.end(), [this](const TypeID& type) {
        return cmptTraits.GetTypes().contains(type);
      }) == cmpts.end());

  const std::size_t numChunk = chunks.size();
  const std::size_t numType = cmpts.size();
  const std::size_t entityIdx = static_cast<std::size_t>(
      std::distance(cmptTraits.GetTypes().begin(),
                    cmptTraits.GetTypes().find(TypeID_of<Entity>)));
  const std::size_t offsetEntity = offsets[entityIdx];

  My::small_vector<My::small_vector<CmptAccessPtr>> chunkCmpts(numChunk);
  My::small_vector<Entity*> chunkEntity(numChunk);

  for (std::size_t i = 0; i < numChunk; i++) {
    Chunk* chunk = chunks[i];
    std::uint8_t* data = chunk->data;
    chunkCmpts[i].reserve(numType);
    for (const auto& type : cmpts) {
      const std::size_t idx = static_cast<std::size_t>(std::distance(
          cmptTraits.GetTypes().begin(), cmptTraits.GetTypes().find(type)));
      const std::size_t offset = offsets[idx];
      chunkCmpts[i].emplace_back(type, data + offset);
      if (type.GetAccessMode() == AccessMode::WRITE)
        chunk->GetHead()->GetCmptInfos()[idx].version = version;
    }
    chunkEntity[i] = reinterpret_cast<Entity*>(data + offsetEntity);
  }

  My::small_vector<std::size_t> sizes;
  sizes.reserve(numType);
  for (const auto& type : cmpts)
    sizes.push_back(cmptTraits.GetTrait(type).size);

  return {chunkEntity, chunkCmpts, sizes};
}

std::size_t Archetype::Erase(EntityAddress addr) {
  Chunk* chunk = chunks[addr.chunkIdx];

  std::size_t movedEntityIdx = chunk->Erase(addr.idxInChunk);

  if (chunk->Empty()) {
    nonFullChunks.erase(addr.chunkIdx);
    chunk->~Chunk();
    chunkAllocator.deallocate(chunk, 1);
  }

  entityNum--;

  return movedEntityIdx;
}

vector<CmptAccessPtr> Archetype::Components(EntityAddress addr,
                                            AccessMode mode) const {
  vector<CmptAccessPtr> rst;

  for (const auto& type : cmptTraits.GetTypes()) {
    if (type.Is<Entity>()) continue;
    rst.push_back(At({type, mode}, addr));
  }

  return rst;
}

My::small_flat_set<My::TypeID> Archetype::GenTypeIDSet(
    std::span<const TypeID> types) {
  small_flat_set<TypeID> sorted_types(types.begin(), types.end());
  sorted_types.insert(TypeID_of<Entity>);
  return sorted_types;
}

void Archetype::NewFrame() {
  for (Chunk* chunk : chunks)
    new (&chunk->GetHead()->chunk_unsync_frame_rsrc)
        std::pmr::monotonic_buffer_resource(sync_frame_rsrc);
}

void Archetype::UpdateVersion(std::uint64_t version) {
  this->version = version;
}
