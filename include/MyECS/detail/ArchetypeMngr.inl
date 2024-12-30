//
// Created by Admin on 26/12/2024.
//

#pragma once

namespace My {
template <typename... Cmpts>
inline Archetype* ArchetypeMngr::GetOrCreateArchetypeOf() {
  auto id = Archetype::ID(TypeList<Cmpts...>{});
  auto target = id2a.find(id);
  if (target == id2a.end()) {
    auto archetype = new Archetype(this, TypeList<Cmpts...>{});
    id2a[id] = archetype;
    ids.insert(archetype->GetID());
    return archetype;
  } else
    return target->second;
}

inline Archetype* ArchetypeMngr::GetArchetypeOf(
    const Archetype::ID& archetypeID) {
  auto target = id2a.find(archetypeID);
  assert(target != id2a.end());
  return target->second;
}

template <typename... Cmpts>
const std::tuple<EntityBase*, Cmpts*...> ArchetypeMngr::CreateEntity() {
  auto entity = entityPool.request();

  Archetype* archetype = GetOrCreateArchetypeOf<Cmpts...>();
  auto [idx, cmpts] = archetype->CreateEntity<Cmpts...>();

  entity->archetype = archetype;
  entity->idx = idx;
  ai2e[{archetype, idx}] = entity;

  // ((entity->RegistCmptFuncs(std::get<Find_v<CmptList, Cmpts>>(cmpts))),...);

  using CmptList = TypeList<Cmpts...>;
  return {entity, std::get<Find_v<CmptList, Cmpts>>(cmpts)...};
}

template <typename... Cmpts>
const std::vector<Archetype*> ArchetypeMngr::GetArchetypeWith() {
  std::vector<Archetype*> rst;
  for (auto& p : id2a) {
    if (p.second->IsContain<Cmpts...>())
      rst.push_back(p.second);
  }
  return rst;
}

template <typename... Cmpts>
const std::tuple<Cmpts*...> ArchetypeMngr::EntityAttach(EntityBase* e) {
  assert(!e->archetype->id.IsContain<Cmpts...>());

  Archetype* srcArchetype = e->archetype;
  size_t srcIdx = e->idx;

  auto& srcID = srcArchetype->GetID();
  auto dstID = srcID;
  dstID.Add<Cmpts...>();

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = id2a.find(dstID);
  if (target == id2a.end()) {
    dstArchetype = Archetype::Add<Cmpts...>::From(srcArchetype);
    assert(dstID == dstArchetype->GetID());
    id2a[dstID] = dstArchetype;
    ids.insert(dstID);
  } else
    dstArchetype = target->second;

  // move src to dst
  size_t dstIdx = dstArchetype->RequestBuffer();

  (new (dstArchetype->At<Cmpts>(dstIdx)) Cmpts, ...);
  for (auto cmptHash : srcID) {
    auto [srcCmpt, srcSize] = srcArchetype->At(cmptHash, srcIdx);
    auto [dstCmpt, dstSize] = dstArchetype->At(cmptHash, dstIdx);
    assert(srcSize == dstSize);
    CmptMngr::Instance().MoveConstruct(cmptHash, dstCmpt, srcCmpt);
  }

  // erase
  auto srcMovedIdx = srcArchetype->Erase(srcIdx);
  if (srcMovedIdx != static_cast<size_t>(-1)) {
    auto srcMovedEntityTarget = ai2e.find({srcArchetype, srcMovedIdx});
    auto srcMovedEntity = srcMovedEntityTarget->second;
    ai2e.erase(srcMovedEntityTarget);
    ai2e[{srcArchetype, srcIdx}] = srcMovedEntity;
    srcMovedEntity->idx = srcMovedIdx;
  }

  ai2e[{dstArchetype, dstIdx}] = e;

  e->archetype = dstArchetype;
  e->idx = dstIdx;

  if (srcArchetype->Size() == 0 && srcArchetype->CmptNum() != 0) {
    ids.erase(srcArchetype->id);
    id2a.erase(srcArchetype->id);
    delete srcArchetype;
  }

  return {dstArchetype->At<Cmpts>(dstIdx)...};
}

template <typename... Cmpts>
void ArchetypeMngr::EntityDetach(EntityBase* e) {
  assert(e->archetype->id.IsContain<Cmpts...>());

  Archetype* srcArchetype = e->archetype;
  size_t srcIdx = e->idx;

  auto& srcID = srcArchetype->GetID();
  auto dstID = srcID;
  dstID.Remove<Cmpts...>();

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = id2a.find(dstID);
  if (target == id2a.end()) {
    dstArchetype = Archetype::Remove<Cmpts...>::From(srcArchetype);
    assert(dstID == dstArchetype->GetID());
    id2a[dstID] = dstArchetype;
    ids.insert(dstID);
  } else
    dstArchetype = target->second;

  // move src to dst
  size_t dstIdx = dstArchetype->RequestBuffer();
  for (auto cmptHash : srcID) {
    auto [srcCmpt, srcSize] = srcArchetype->At(cmptHash, srcIdx);
    if (dstID.IsContain(cmptHash)) {
      auto [dstCmpt, dstSize] = dstArchetype->At(cmptHash, dstIdx);
      assert(srcSize == dstSize);
      CmptMngr::Instance().MoveConstruct(cmptHash, dstCmpt, srcCmpt);
    } else
      CmptMngr::Instance().Destruct(cmptHash, srcCmpt);
  }

  // erase
  auto srcMovedIdx = srcArchetype->Erase(srcIdx);
  if (srcMovedIdx != static_cast<size_t>(-1)) {
    auto srcMovedEntityTarget = ai2e.find({srcArchetype, srcMovedIdx});
    auto srcMovedEntity = srcMovedEntityTarget->second;
    ai2e.erase(srcMovedEntityTarget);
    ai2e[{srcArchetype, srcIdx}] = srcMovedEntity;
    srcMovedEntity->idx = srcMovedIdx;
  }

  ai2e[{dstArchetype, dstIdx}] = e;

  e->archetype = dstArchetype;
  e->idx = dstIdx;

  if (srcArchetype->Size() == 0) {
    ids.erase(srcArchetype->id);
    id2a.erase(srcArchetype->id);
    delete srcArchetype;
  }
}
}  // namespace My