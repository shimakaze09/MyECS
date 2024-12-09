#ifndef ARCHETYPE_MANAGER_INL
#define ARCHETYPE_MANAGER_INL

namespace My {
template <typename... Cmpts>
inline Archetype* ArchetypeManager::GetOrCreateArchetypeOf() {
  auto id = Archetype::ID(TypeList<Cmpts...>{});
  auto target = m_idToArchetype.find(id);
  if (target == m_idToArchetype.end()) {
    auto archetype = new Archetype(this, TypeList<Cmpts...>{});
    m_idToArchetype[id] = archetype;
    m_ids.insert(archetype->GetID());
    return archetype;
  } else
    return target->second;
}

template <typename... Cmpts>
const std::tuple<EntityData*, Cmpts*...> ArchetypeManager::CreateEntity() {
  auto entity = m_entityPool.Request();

  Archetype* archetype = GetOrCreateArchetypeOf<Cmpts...>();
  auto [idx, cmpts] = archetype->CreateEntity<Cmpts...>(entity);

  entity->archetype() = archetype;
  entity->idx() = idx;
  m_dataToPointer[*entity] = entity;

  using CmptList = TypeList<Cmpts...>;
  return {entity, std::get<Find_v<CmptList, Cmpts>>(cmpts)...};
}

template <typename... Cmpts>
const std::vector<Archetype*> ArchetypeManager::GetArchetypeWith() {
  std::vector<Archetype*> rst;
  for (auto& p : m_idToArchetype) {
    if (p.second->IsContain<Cmpts...>())
      rst.push_back(p.second);
  }
  return rst;
}

template <typename... Cmpts>
const std::tuple<Cmpts*...> ArchetypeManager::EntityAttach(EntityData* e) {
  assert(!e->archetype()->id.IsContain<Cmpts...>());

  Archetype* srcArchetype = e->archetype();
  size_t srcIdx = e->idx();

  auto& srcID = srcArchetype->GetID();
  auto dstID = srcID;
  dstID.Add<Cmpts...>();

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = m_idToArchetype.find(dstID);
  if (target == m_idToArchetype.end()) {
    dstArchetype = Archetype::Add<Cmpts...>::From(srcArchetype);
    assert(dstID == dstArchetype->GetID());
    m_idToArchetype[dstID] = dstArchetype;
    m_ids.insert(dstID);
  } else
    dstArchetype = target->second;

  // move src to dst
  size_t dstIdx = dstArchetype->CreateEntity();

  (dstArchetype->New<Cmpts>(dstIdx, e), ...);
  for (auto cmptHash : srcID) {
    auto [srcCmpt, srcSize] = srcArchetype->At(cmptHash, srcIdx);
    auto [dstCmpt, dstSize] = dstArchetype->At(cmptHash, dstIdx);
    e->MoveCmpt(srcCmpt, dstCmpt);
    assert(srcSize == dstSize);
    memcpy(dstCmpt, srcCmpt, srcSize);
  }

  // erase
  auto [srcMovedIdx, pairs] = srcArchetype->Erase(srcIdx);
  if (srcMovedIdx != static_cast<size_t>(-1)) {
    auto srcMovedEntityTarget =
        m_dataToPointer.find({srcArchetype, srcMovedIdx});
    auto srcMovedEntity = srcMovedEntityTarget->second;
    for (auto p : pairs)
      srcMovedEntity->MoveCmpt(p.first, p.second);
    m_dataToPointer.erase(srcMovedEntityTarget);
    m_dataToPointer[{srcArchetype, srcIdx}] = srcMovedEntity;
    srcMovedEntity->idx() = srcMovedIdx;
  }

  m_dataToPointer[{dstArchetype, dstIdx}] = e;

  e->archetype() = dstArchetype;
  e->idx() = dstIdx;

  if (srcArchetype->Size() == 0 && srcArchetype->CmptNum() != 0) {
    m_ids.erase(srcArchetype->id);
    m_idToArchetype.erase(srcArchetype->id);
    delete srcArchetype;
  }

  return {dstArchetype->At<Cmpts>(dstIdx)...};
}

template <typename... Cmpts>
void ArchetypeManager::EntityDetach(EntityData* e) {
  assert(e->archetype()->id.IsContain<Cmpts...>());

  Archetype* srcArchetype = e->archetype();
  size_t srcIdx = e->idx();

  auto& srcID = srcArchetype->GetID();
  auto dstID = srcID;
  dstID.Remove<Cmpts...>();

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = m_idToArchetype.find(dstID);
  if (target == m_idToArchetype.end()) {
    dstArchetype = Archetype::Remove<Cmpts...>::From(srcArchetype);
    assert(dstID == dstArchetype->GetID());
    m_idToArchetype[dstID] = dstArchetype;
    m_ids.insert(dstID);
  } else
    dstArchetype = target->second;

  // move src to dst
  size_t dstIdx = dstArchetype->CreateEntity();
  for (auto cmptHash : srcID) {
    auto [srcCmpt, srcSize] = srcArchetype->At(cmptHash, srcIdx);
    if (dstID.IsContain(cmptHash)) {
      auto [dstCmpt, dstSize] = dstArchetype->At(cmptHash, dstIdx);
      e->MoveCmpt(srcCmpt, dstCmpt);
      assert(srcSize == dstSize);
      memcpy(dstCmpt, srcCmpt, srcSize);
    } else
      e->ReleaseCmpt(srcCmpt);
  }

  // erase
  auto [srcMovedIdx, pairs] = srcArchetype->Erase(srcIdx);
  if (srcMovedIdx != static_cast<size_t>(-1)) {
    auto srcMovedEntityTarget =
        m_dataToPointer.find({srcArchetype, srcMovedIdx});
    auto srcMovedEntity = srcMovedEntityTarget->second;
    for (auto p : pairs)
      srcMovedEntity->MoveCmpt(p.first, p.second);
    m_dataToPointer.erase(srcMovedEntityTarget);
    m_dataToPointer[{srcArchetype, srcIdx}] = srcMovedEntity;
    srcMovedEntity->idx() = srcMovedIdx;
  }

  m_dataToPointer[{dstArchetype, dstIdx}] = e;

  e->archetype() = dstArchetype;
  e->idx() = dstIdx;

  if (srcArchetype->Size() == 0) {
    m_ids.erase(srcArchetype->id);
    m_idToArchetype.erase(srcArchetype->id);
    delete srcArchetype;
  }
}
}  // namespace My

#endif  // ARCHETYPE_MANAGER_INL