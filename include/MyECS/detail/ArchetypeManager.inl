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
EntityData* ArchetypeManager::CreateEntity() {
  Archetype* archetype = GetOrCreateArchetypeOf<Cmpts...>();
  size_t idx = archetype->CreateEntity<Cmpts...>();

  auto entity = m_entityPool.Request();
  entity->archetype() = archetype;
  entity->idx() = idx;
  m_dataToPointer[*entity] = entity;

  return entity;
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

template <typename Cmpt, typename... Args>
Cmpt* ArchetypeManager::EntityAttach(EntityData* e, Args&&... args) {
  assert(!e->archetype()->id.IsContain<Cmpt>());

  Archetype* srcArchetype = e->archetype();
  size_t srcIdx = e->idx();

  auto& srcID = srcArchetype->GetID();
  auto dstID = srcID;
  dstID.Add<Cmpt>();

  Archetype* dstArchetype;
  auto target = m_idToArchetype.find(dstID);
  if (target == m_idToArchetype.end()) {
    dstArchetype = Archetype::Add<Cmpt>::From(srcArchetype);
    assert(dstID == dstArchetype->GetID());
    m_idToArchetype[dstID] = dstArchetype;
    m_ids.insert(dstID);
  } else
    dstArchetype = target->second;

  size_t dstIdx = dstArchetype->CreateEntity();
  Cmpt* cmpt = dstArchetype->Init<Cmpt>(dstIdx, std::forward<Args>(args)...);
  for (auto cmptHash : srcID) {
    auto srcCmpt = srcArchetype->At(cmptHash, srcIdx);
    auto dstCmpt = dstArchetype->At(cmptHash, dstIdx);
    size_t size = std::get<1>(srcCmpt);
    assert(size == std::get<1>(dstCmpt));
    memcpy(std::get<0>(dstCmpt), std::get<0>(srcCmpt), size);
  }

  size_t srcMovedIdx = srcArchetype->Erase(srcIdx);
  if (srcMovedIdx != static_cast<size_t>(-1)) {
    auto srcMovedEntityTarget =
        m_dataToPointer.find({srcArchetype, srcMovedIdx});
    auto srcMovedEntity = srcMovedEntityTarget->second;
    m_dataToPointer.erase(srcMovedEntityTarget);
    m_dataToPointer[{srcArchetype, srcIdx}] = srcMovedEntity;
    srcMovedEntity->idx() = srcMovedIdx;
  }

  m_dataToPointer[{dstArchetype, dstIdx}] = e;

  e->archetype() = dstArchetype;
  e->idx() = dstIdx;

  return cmpt;
}

template <typename Cmpt>
void ArchetypeManager::EntityDetach(EntityData* e) {
  assert(e->archetype()->id.IsContain<Cmpt>());

  Archetype* srcArchetype = e->archetype();
  size_t srcIdx = e->idx();

  auto& srcID = srcArchetype->GetID();
  auto dstID = srcID;
  dstID.Remove<Cmpt>();

  Archetype* dstArchetype;
  auto target = m_idToArchetype.find(dstID);
  if (target == m_idToArchetype.end()) {
    dstArchetype = Archetype::Remove<Cmpt>::From(srcArchetype);
    assert(dstID == dstArchetype->GetID());
    m_idToArchetype[dstID] = dstArchetype;
    m_ids.insert(dstID);
  } else
    dstArchetype = target->second;

  size_t dstIdx = dstArchetype->CreateEntity();
  for (auto cmptHash : dstID) {
    auto srcCmpt = srcArchetype->At(cmptHash, srcIdx);
    auto dstCmpt = dstArchetype->At(cmptHash, dstIdx);
    size_t size = std::get<1>(srcCmpt);
    assert(size == std::get<1>(dstCmpt));
    memcpy(std::get<0>(dstCmpt), std::get<0>(srcCmpt), size);
  }

  size_t srcMovedIdx = srcArchetype->Erase(srcIdx);
  if (srcMovedIdx != static_cast<size_t>(-1)) {
    auto srcMovedEntityTarget =
        m_dataToPointer.find({srcArchetype, srcMovedIdx});
    auto srcMovedEntity = srcMovedEntityTarget->second;
    m_dataToPointer.erase(srcMovedEntityTarget);
    m_dataToPointer[{srcArchetype, srcIdx}] = srcMovedEntity;
    srcMovedEntity->idx() = srcMovedIdx;
  }

  m_dataToPointer[{dstArchetype, dstIdx}] = e;

  e->archetype() = dstArchetype;
  e->idx() = dstIdx;
}

}  // namespace My

#endif  // ARCHETYPE_MANAGER_INL