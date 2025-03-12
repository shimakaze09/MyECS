//
// Created by Admin on 26/12/2024.
//

#include <MyECS/detail/Archetype.h>

using namespace My::MyECS;
using namespace std;

void Archetype::SetLayout() {
  vector<size_t> alignments;
  vector<size_t> sizes;

  for (const auto& type : types) {
    alignments.push_back(cmptTraits.Alignof(type));
    sizes.push_back(cmptTraits.Sizeof(type));
  }

  auto layout = Chunk::GenLayout(alignments, sizes);
  chunkCapacity = layout.capacity;

  size_t i = 0;
  for (const auto& type : types)
    type2offset[type] = layout.offsets[i++];
}

Archetype* Archetype::New(const CmptType* types, size_t num) {
  auto rst = new Archetype;
  rst->types = CmptTypeSet{types, num};
  rst->types.Insert(CmptType::Of<Entity>);
  rst->cmptTraits.Register<Entity>();
  for (size_t i = 0; i < num; i++)
    rst->cmptTraits.Register(types[i]);
  rst->SetLayout();
  return rst;
}

Archetype* Archetype::Add(const Archetype* from, const CmptType* types,
                          size_t num) {
#ifndef NDEBUG
  for (size_t i = 0; i < num; i++)
    assert(!from->types.Contains(types[i]));
#endif  // !NDEBUG

  Archetype* rst = new Archetype;

  rst->types = from->types;
  rst->cmptTraits = from->cmptTraits;
  for (size_t i = 0; i < num; i++) {
    rst->types.Insert(types[i]);
    rst->cmptTraits.Register(types[i]);
  }

  rst->SetLayout();

  return rst;
}

Archetype* Archetype::Remove(const Archetype* from, const CmptType* types,
                             size_t num) {
  for (size_t i = 0; i < num; i++)
    assert(from->types.Contains(types[i]));

  Archetype* rst = new Archetype;

  rst->types = from->types;
  rst->cmptTraits = from->cmptTraits;
  for (size_t i = 0; i < num; i++) {
    rst->types.Erase(types[i]);
    rst->cmptTraits.Deregister(types[i]);
  }

  rst->SetLayout();

  return rst;
}

size_t Archetype::Create(Entity e) {
  size_t idx = RequestBuffer();
  size_t idxInChunk = idx % chunkCapacity;
  byte* buffer = chunks[idx / chunkCapacity]->Data();

  const auto& rtdct = RTDCmptTraits::Instance();
  for (auto type : types) {
    if (type.Is<Entity>()) {
      constexpr size_t size = sizeof(Entity);
      size_t offset = Offsetof(type);
      memcpy(buffer + offset + idxInChunk * size, &e, size);
    } else {
      auto target = rtdct.default_constructors.find(type);
      if (target != rtdct.default_constructors.end()) {
        size_t size = cmptTraits.Sizeof(type);
        size_t offset = Offsetof(type);
        byte* dst = buffer + offset + idxInChunk * size;
        target->second(dst);
      }
    }
  }

  return idx;
}

Archetype::~Archetype() {
  for (const auto& type : types) {
    size_t size = cmptTraits.Sizeof(type);
    size_t offset = Offsetof(type);
    for (size_t i = 0; i < entityNum; i++) {
      size_t idxInChunk = i % chunkCapacity;
      byte* buffer = chunks[i / chunkCapacity]->Data();
      byte* address = buffer + offset + idxInChunk * size;
      cmptTraits.Destruct(type, address);
    }
  }
  for (Chunk* chunk : chunks)
    sharedChunkPool.Recycle(chunk);
}

size_t Archetype::RequestBuffer() {
  if (entityNum == chunks.size() * chunkCapacity) {
    auto chunk = sharedChunkPool.Request();
    chunks.push_back(chunk);
  }
  return entityNum++;
}

void* Archetype::At(CmptType type, size_t idx) const {
  assert(idx < entityNum);
  assert(types.Contains(type));

  size_t size = cmptTraits.Sizeof(type);
  size_t offset = Offsetof(type);
  size_t idxInChunk = idx % chunkCapacity;
  byte* buffer = chunks[idx / chunkCapacity]->Data();

  return buffer + offset + idxInChunk * size;
}

size_t Archetype::Instantiate(Entity e, size_t srcIdx) {
  assert(srcIdx < entityNum);
  size_t dstIdx = RequestBuffer();

  size_t srcIdxInChunk = srcIdx % chunkCapacity;
  byte* srcBuffer = chunks[srcIdx / chunkCapacity]->Data();
  size_t dstIdxInChunk = dstIdx % chunkCapacity;
  byte* dstBuffer = chunks[dstIdx / chunkCapacity]->Data();

  for (auto type : types) {
    size_t offset = Offsetof(type);

    if (type.Is<Entity>()) {
      constexpr size_t size = sizeof(Entity);
      memcpy(dstBuffer + offset + dstIdxInChunk * size, &e, size);
    } else {
      size_t size = cmptTraits.Sizeof(type);
      byte* dst = dstBuffer + offset + dstIdxInChunk * size;
      byte* src = srcBuffer + offset + srcIdxInChunk * size;

      cmptTraits.CopyConstruct(type, dst, src);
    }
  }

  return dstIdx;
}

tuple<vector<Entity*>, vector<vector<void*>>, vector<size_t>> Archetype::Locate(
    const set<CmptType>& cmptTypes) const {
  assert(types.Contains(cmptTypes));
  vector<vector<void*>> chunkCmpts(chunks.size());
  vector<Entity*> chunkEntity;

  for (size_t i = 0; i < chunks.size(); i++) {
    auto data = chunks[i]->Data();
    for (const auto& type : cmptTypes)
      chunkCmpts[i].push_back(data + Offsetof(type));
    chunkEntity.push_back(
        reinterpret_cast<Entity*>(data + Offsetof(CmptType::Of<Entity>)));
  }

  vector<size_t> sizes;

  for (const auto& type : cmptTypes)
    sizes.push_back(cmptTraits.Sizeof(type));

  return {chunkEntity, chunkCmpts, sizes};
}

size_t Archetype::Erase(size_t idx) {
  assert(idx < entityNum);

  size_t dstIdxInChunk = idx % chunkCapacity;
  byte* dstBuffer = chunks[idx / chunkCapacity]->Data();

  size_t movedIdx = size_t_invalid;

  if (idx != entityNum - 1) {
    size_t movedIdxInArchetype = entityNum - 1;

    size_t srcIdxInChunk = movedIdxInArchetype % chunkCapacity;
    byte* srcBuffer = chunks[movedIdxInArchetype / chunkCapacity]->Data();

    for (auto type : types) {
      auto size = cmptTraits.Sizeof(type);
      auto offset = Offsetof(type);
      byte* dst = dstBuffer + offset + dstIdxInChunk * size;
      byte* src = srcBuffer + offset + srcIdxInChunk * size;

      if (type.Is<Entity>())
        movedIdx = reinterpret_cast<Entity*>(dst)->Idx();

      cmptTraits.Destruct(type, dst);
      cmptTraits.MoveConstruct(type, dst, src);
    }
  } else {
    for (auto type : types) {
      auto size = cmptTraits.Sizeof(type);
      auto offset = Offsetof(type);
      byte* dst = dstBuffer + offset + dstIdxInChunk * size;
      cmptTraits.Destruct(type, dst);
    }
  }

  entityNum--;

  if (chunks.size() * chunkCapacity - entityNum >= chunkCapacity) {
    Chunk* chunk = chunks.back();
    sharedChunkPool.Recycle(chunk);
    chunks.pop_back();
  }

  return movedIdx;
}

vector<CmptPtr> Archetype::Components(size_t idx) const {
  vector<CmptPtr> rst;

  for (const auto& type : types) {
    if (type.Is<Entity>())
      continue;
    rst.emplace_back(type, At(type, idx));
  }

  return rst;
}
