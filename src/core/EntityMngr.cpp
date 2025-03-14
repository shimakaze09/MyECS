//
// Created by Admin on 26/12/2024.
//

#include <MyECS/EntityMngr.h>
#include <MyECS/IListener.h>

using namespace My::MyECS;
using namespace std;

size_t EntityMngr::RequestEntityFreeEntry() {
  if (entityTableFreeEntry.empty()) {
    size_t index = entityTable.size();
    entityTable.emplace_back();
    return index;
  }

  size_t entry = entityTableFreeEntry.back();
  entityTableFreeEntry.pop_back();
  return entry;
}

void EntityMngr::RecycleEntityEntry(Entity e) {
  assert(Exist(e));

  auto& info = entityTable[e.Idx()];
  info.archetype = nullptr;
  info.idxInArchetype = size_t_invalid;
  info.version++;

  entityTableFreeEntry.push_back(e.Idx());
}

Archetype* EntityMngr::GetOrCreateArchetypeOf(const CmptType* types,
                                              size_t num) {
  auto typeset = Archetype::GenCmptTypeSet(types, num);
  auto target = ts2a.find(typeset);
  if (target != ts2a.end())
    return target->second.get();

  auto archetype = Archetype::New(types, num);

  ts2a.emplace(std::move(typeset), std::unique_ptr<Archetype>{archetype});
  for (auto& [query, archetypes] : queryCache) {
    if (archetype->GetCmptTypeSet().IsMatch(query))
      archetypes.insert(archetype);
  }

  return archetype;
}

Entity EntityMngr::Create(const CmptType* types, size_t num) {
  Archetype* archetype = GetOrCreateArchetypeOf(types, num);
  size_t entityIndex = RequestEntityFreeEntry();
  EntityInfo& info = entityTable[entityIndex];
  Entity e{entityIndex, info.version};
  info.archetype = archetype;
  info.idxInArchetype = archetype->Create(e);
  return e;
}

void EntityMngr::AttachWithoutInit(Entity e, const CmptType* types,
                                   size_t num) {
  assert(types != nullptr && num > 0);
  assert(IsSet(types, num));
  if (!Exist(e))
    throw std::invalid_argument("Entity is invalid");

  auto& info = entityTable[e.Idx()];
  Archetype* srcArchetype = info.archetype;
  size_t srcIdxInArchetype = info.idxInArchetype;

  const auto& srcCmptTypeSet = srcArchetype->GetCmptTypeSet();
  auto dstCmptTypeSet = srcCmptTypeSet;
  dstCmptTypeSet.Insert(types, num);

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = ts2a.find(dstCmptTypeSet);
  if (target == ts2a.end()) {
    dstArchetype = Archetype::Add(srcArchetype, types, num);
    assert(dstCmptTypeSet == dstArchetype->GetCmptTypeSet());
    for (auto& [query, archetypes] : queryCache) {
      if (dstCmptTypeSet.IsMatch(query))
        archetypes.insert(dstArchetype);
    }
    ts2a.emplace(std::move(dstCmptTypeSet),
                 std::unique_ptr<Archetype>{dstArchetype});
  } else
    dstArchetype = target->second.get();

  if (dstArchetype == srcArchetype)
    return;

  // move src to dst
  size_t dstIdxInArchetype = dstArchetype->RequestBuffer();

  const auto& srcCmptTraits = srcArchetype->GetRTSCmptTraits();
  for (const auto& type : srcCmptTypeSet.data) {
    auto srcCmpt = srcArchetype->At(type, srcIdxInArchetype);
    auto dstCmpt = dstArchetype->At(type, dstIdxInArchetype);
    srcCmptTraits.MoveConstruct(type, dstCmpt, srcCmpt);
  }

  // erase
  auto srcMovedEntityIndex = srcArchetype->Erase(srcIdxInArchetype);
  if (srcMovedEntityIndex != size_t_invalid)
    entityTable[srcMovedEntityIndex].idxInArchetype = srcIdxInArchetype;

  info.archetype = dstArchetype;
  info.idxInArchetype = dstIdxInArchetype;
}

void EntityMngr::Attach(Entity e, const CmptType* types, size_t num) {
  auto srcArchetype = entityTable[e.Idx()].archetype;
  AttachWithoutInit(e, types, num);
  const auto& new_info = entityTable[e.Idx()];
  const auto& rtdct = RTDCmptTraits::Instance();
  for (size_t i = 0; i < num; i++) {
    const auto& type = types[i];
    if (srcArchetype->GetCmptTypeSet().Contains(type))
      continue;
    auto target = rtdct.default_constructors.find(type);
    if (target == rtdct.default_constructors.end())
      continue;

    target->second(new_info.archetype->At(type, new_info.idxInArchetype));
  }
}

void EntityMngr::Detach(Entity e, const CmptType* types, size_t num) {
  assert(types != nullptr && num > 0);
  assert(IsSet(types, num));
  if (!Exist(e))
    throw std::invalid_argument("EntityMngr::Detach: Entity is invalid");

  auto& info = entityTable[e.Idx()];
  Archetype* srcArchetype = info.archetype;
  size_t srcIdxInArchetype = info.idxInArchetype;

  const auto& srcCmptTypeSet = srcArchetype->GetCmptTypeSet();
  auto dstCmptTypeSet = srcCmptTypeSet;
  dstCmptTypeSet.Erase(types, num);

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = ts2a.find(dstCmptTypeSet);
  if (target == ts2a.end()) {
    dstArchetype = Archetype::Remove(srcArchetype, types, num);
    assert(dstCmptTypeSet == dstArchetype->GetCmptTypeSet());
    for (auto& [query, archetypes] : queryCache) {
      if (dstCmptTypeSet.IsMatch(query))
        archetypes.insert(dstArchetype);
    }
    ts2a.emplace(std::move(dstCmptTypeSet),
                 std::unique_ptr<Archetype>{dstArchetype});
  } else
    dstArchetype = target->second.get();

  if (dstArchetype == srcArchetype)
    return;

  // move src to dst
  size_t dstIdxInArchetype = dstArchetype->RequestBuffer();
  const auto& srcCmptTraits = srcArchetype->GetRTSCmptTraits();
  for (const auto& type : srcCmptTypeSet.data) {
    auto srcCmpt = srcArchetype->At(type, srcIdxInArchetype);
    if (dstCmptTypeSet.Contains(type)) {
      auto dstCmpt = dstArchetype->At(type, dstIdxInArchetype);
      srcCmptTraits.MoveConstruct(type, dstCmpt, srcCmpt);
    }
    // no need to call destructor because we will erase it later
    /*else
			srcCmptTraits.Destruct(type, srcCmpt);*/
  }

  // erase
  auto srcMovedEntityIndex =
      srcArchetype->Erase(srcIdxInArchetype);  // call destructor
  if (srcMovedEntityIndex != size_t_invalid)
    entityTable[srcMovedEntityIndex].idxInArchetype = srcIdxInArchetype;

  info.archetype = dstArchetype;
  info.idxInArchetype = dstIdxInArchetype;
}

vector<CmptPtr> EntityMngr::Components(Entity e) const {
  if (!Exist(e))
    throw std::invalid_argument("Entity is invalid");

  const auto& info = entityTable[e.Idx()];
  return info.archetype->Components(info.idxInArchetype);
}

Entity EntityMngr::Instantiate(Entity srcEntity) {
  if (!Exist(srcEntity))
    throw std::invalid_argument("Entity is invalid");

  size_t dstEntityIndex = RequestEntityFreeEntry();
  const auto& srcInfo = entityTable[srcEntity.Idx()];
  auto& dstInfo = entityTable[dstEntityIndex];
  Entity dstEntity{dstEntityIndex, dstInfo.version};
  size_t dstIndexInArchetype =
      srcInfo.archetype->Instantiate(dstEntity, srcInfo.idxInArchetype);
  dstInfo.archetype = srcInfo.archetype;
  dstInfo.idxInArchetype = dstIndexInArchetype;
  return dstEntity;
}

bool EntityMngr::IsSet(const CmptType* types, size_t num) noexcept {
  for (size_t i = 0; i < num; i++) {
    for (size_t j = 0; j < i; j++)
      if (types[i] == types[j])
        return false;
  }

  return true;
}

const set<Archetype*>& EntityMngr::QueryArchetypes(
    const EntityQuery& query) const {
  auto target = queryCache.find(query);
  if (target != queryCache.end())
    return target->second;

  // queryCache is **mutable**
  set<Archetype*>& archetypes = queryCache[query];
  for (const auto& [ts, a] : ts2a) {
    if (a->GetCmptTypeSet().IsMatch(query))
      archetypes.insert(a.get());
  }

  return archetypes;
}

size_t EntityMngr::EntityNum(const EntityQuery& query) const {
  size_t sum = 0;
  for (const auto& archetype : QueryArchetypes(query))
    sum += archetype->EntityNum();
  return sum;
}

void EntityMngr::Destroy(Entity e) {
  if (!Exist(e))
    throw std::invalid_argument("Entity is invalid");

  auto info = entityTable[e.Idx()];
  auto archetype = info.archetype;
  auto idxInArchetype = info.idxInArchetype;

  auto movedEntityIndex = archetype->Erase(idxInArchetype);

  if (movedEntityIndex != size_t_invalid)
    entityTable[movedEntityIndex].idxInArchetype = idxInArchetype;

  RecycleEntityEntry(e);
}

tuple<bool, vector<CmptPtr>> EntityMngr::LocateSingletons(
    const SingletonLocator& locator) const {
  size_t numSingletons = 0;
  vector<CmptPtr> rst;
  rst.reserve(locator.SingletonTypes().size());
  for (const auto& t : locator.SingletonTypes()) {
    auto ptr = GetIfSingleton(t);
    if (ptr.Ptr() == nullptr)
      return {false, {}};
    rst.push_back(ptr);
  }
  return {true, rst};
}

void EntityMngr::GenEntityJob(World* w, Job* job, SystemFunc* sys) const {
  assert(sys->GetMode() == SystemFunc::Mode::Entity);

  auto [success, singletons] = LocateSingletons(sys->singletonLocator);
  if (!success)
    return;

  size_t indexOffsetInQuery = 0;
  for (Archetype* archetype : QueryArchetypes(sys->entityQuery)) {
    auto [chunkEntity, chunkCmpts, sizes] =
        archetype->Locate(sys->entityQuery.locator);

    size_t num = archetype->EntityNum();
    size_t chunkNum = archetype->ChunkNum();
    size_t chunkCapacity = archetype->ChunkCapacity();

    for (size_t i = 0; i < chunkNum; i++) {
      job->emplace([=, sizes = sizes, entities = chunkEntity[i],
                    cmpts = move(chunkCmpts[i]),
                    singletons = singletons]() mutable {
        size_t idxOffsetInChunk = i * chunkCapacity;
        size_t indexOffsetInQueryChunk = indexOffsetInQuery + idxOffsetInChunk;

        size_t J = min(chunkCapacity, num - idxOffsetInChunk);
        for (size_t j = 0; j < J; j++) {
          (*sys)(w, SingletonsView{singletons.data(), singletons.size()},
                 entities[j], indexOffsetInQueryChunk + j,
                 CmptsView{cmpts.data(), cmpts.size()});
          for (size_t k = 0; k < cmpts.size(); k++)
            reinterpret_cast<uint8_t*&>(cmpts[k]) += sizes[k];
        }
      });
    }

    indexOffsetInQuery += num;
  }
}

void EntityMngr::GenChunkJob(World* w, Job* job, SystemFunc* sys) const {
  assert(sys->GetMode() == SystemFunc::Mode::Chunk);

  auto [success, singletons] = LocateSingletons(sys->singletonLocator);
  if (!success)
    return;

  for (Archetype* archetype : QueryArchetypes(sys->entityQuery)) {
    size_t chunkNum = archetype->ChunkNum();

    for (size_t i = 0; i < chunkNum; i++) {
      job->emplace([=, singletons = singletons]() {
        (*sys)(w, SingletonsView{singletons.data(), singletons.size()},
               ChunkView{archetype, i});
      });
    }
  }
}

void EntityMngr::GenJob(World* w, Job* job, SystemFunc* sys) const {
  assert(sys->GetMode() == SystemFunc::Mode::Job);

  auto [success, singletons] = LocateSingletons(sys->singletonLocator);
  if (!success)
    return;

  job->emplace([=, singletons = std::move(singletons)]() {
    (*sys)(w, SingletonsView{singletons.data(), singletons.size()});
  });
}

void EntityMngr::Accept(IListener* listener) const {
  listener->EnterEntityMngr(this);
  for (const auto& [ts, a] : ts2a) {
    for (size_t i = 0; i < a->EntityNum(); i++) {
      auto e = a->At<Entity>(i);
      listener->EnterEntity(e);
      for (const auto& cmpt : a->Components(i)) {
        listener->EnterCmptPtr(&cmpt);
        listener->ExistCmptPtr(&cmpt);
      }
      listener->ExistEntity(e);
    }
  }
  listener->ExistEntityMngr(this);
}

bool EntityMngr::IsSingleton(CmptType t) const {
  ArchetypeFilter filter{{t}, {}, {}};
  EntityQuery query(move(filter));
  const auto& archetypes = QueryArchetypes(query);
  if (archetypes.size() != 1)
    return false;
  auto archetype = *archetypes.begin();
  if (archetype->EntityNum() != 1)
    return false;

  return true;
}

Entity EntityMngr::GetSingletonEntity(CmptType t) const {
  assert(IsSingleton(t));
  ArchetypeFilter filter{{t}, {}, {}};
  EntityQuery query(move(filter));
  const auto& archetypes = QueryArchetypes(query);
  auto archetype = *archetypes.begin();
  return *archetype->At<Entity>(0);
}

CmptPtr EntityMngr::GetSingleton(CmptType t) const {
  assert(IsSingleton(t));
  ArchetypeFilter filter{{t}, {}, {}};
  EntityQuery query(move(filter));
  const auto& archetypes = QueryArchetypes(query);
  auto archetype = *archetypes.begin();
  return {t, archetype->At(t, 0)};
}

CmptPtr EntityMngr::GetIfSingleton(CmptType t) const {
  ArchetypeFilter filter{{t}, {}, {}};
  EntityQuery query(move(filter));
  const auto& archetypes = QueryArchetypes(query);
  if (archetypes.size() != 1)
    return {CmptType::Invalid(), nullptr};
  auto archetype = *archetypes.begin();
  if (archetype->EntityNum() != 1)
    return {CmptType::Invalid(), nullptr};

  return {t, archetype->At(t, 0)};
}
