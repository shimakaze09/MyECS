//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "../CmptTag.h"

#include <MyTemplate/Func.h>
#include <MyTemplate/Typelist.h>

#include <stdexcept>

namespace My {
template <typename... Cmpts>
Archetype* EntityMngr::GetOrCreateArchetypeOf() {
  static_assert(
      IsSet_v<TypeList<Entity, Cmpts...>>,
      "EntityMngr::GetOrCreateArchetypeOf: <Cmpts> must be different");

  constexpr size_t hashcode = Archetype::HashCode<Cmpts...>();
  auto target = h2a.find(hashcode);
  if (target != h2a.end())
    return target->second;

  auto archetype = new Archetype(TypeList<Cmpts...>{});
  h2a[hashcode] = archetype;
  for (auto& [query, archetypes] : queryCache) {
    if (archetype->GetCmptTypeSet().IsMatch(query))
      archetypes.insert(archetype);
  }

  return archetype;
}

template <typename... Cmpts>
const std::tuple<Entity, Cmpts*...> EntityMngr::CreateEntity() {
  static_assert(IsSet_v<TypeList<Entity, Cmpts...>>,
                "EntityMngr::CreateEntity: <Cmpts> must be different");
  Archetype* archetype = GetOrCreateArchetypeOf<Cmpts...>();
  size_t entityIndex = RequestEntityFreeEntry();
  EntityInfo& info = entityTable[entityIndex];
  Entity e{entityIndex, info.version};
  info.archetype = archetype;
  auto [idxInArchetype, cmpts] = archetype->CreateEntity<Cmpts...>(e);
  info.idxInArchetype = idxInArchetype;
  return {e, std::get<Cmpts*>(cmpts)...};
}

template <typename... Cmpts>
const std::tuple<Cmpts*...> EntityMngr::AttachWithoutInit(Entity e) {
  static_assert(sizeof...(Cmpts) > 0,
                "EntityMngr::AttachWithoutInit: sizeof...(<Cmpts>) > 0");
  static_assert(IsSet_v<TypeList<Entity, Cmpts...>>,
                "EntityMngr::AttachWithoutInit: <Cmpts> must be different");
  assert("EntityMngr::AttachWithoutInit: e is invalid" && Exist(e));

  auto& info = entityTable[e.Idx()];
  Archetype* srcArchetype = info.archetype;
  size_t srcIdxInArchetype = info.idxInArchetype;

  assert((srcArchetype->types.IsNotContain<Cmpts>() && ...));

  auto& srcCmptTypeSet = srcArchetype->GetCmptTypeSet();
  auto dstCmptTypeSet = srcCmptTypeSet;
  dstCmptTypeSet.Add<Cmpts...>();
  size_t dstCmptTypeSetHashCode = dstCmptTypeSet.HashCode();

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = h2a.find(dstCmptTypeSetHashCode);
  if (target == h2a.end()) {
    dstArchetype = Archetype::Add<Cmpts...>(srcArchetype);
    assert(dstCmptTypeSet == dstArchetype->GetCmptTypeSet());
    h2a[dstCmptTypeSetHashCode] = dstArchetype;
    for (auto& [query, archetypes] : queryCache) {
      if (dstCmptTypeSet.IsMatch(query))
        archetypes.insert(dstArchetype);
    }
  } else
    dstArchetype = target->second;

  // move src to dst
  size_t dstIdxInArchetype = dstArchetype->RequestBuffer();

  auto srcCmptTraits = srcArchetype->GetRuntimeCmptTraits();
  for (auto type : srcCmptTypeSet) {
    auto [srcCmpt, srcSize] = srcArchetype->At(type, srcIdxInArchetype);
    auto [dstCmpt, dstSize] = dstArchetype->At(type, dstIdxInArchetype);
    assert(srcSize == dstSize);
    srcCmptTraits.MoveConstruct(type, dstCmpt, srcCmpt);
  }

  // erase
  auto srcMovedEntityIndex = srcArchetype->Erase(srcIdxInArchetype);
  if (srcMovedEntityIndex != Entity::npos)
    entityTable[srcMovedEntityIndex].idxInArchetype = srcIdxInArchetype;

  info.archetype = dstArchetype;
  info.idxInArchetype = dstIdxInArchetype;

  return {dstArchetype->At<Cmpts>(dstIdxInArchetype)...};
}

template <typename... Cmpts>
const std::tuple<Cmpts*...> EntityMngr::Attach(Entity e) {
  static_assert((std::is_constructible_v<Cmpts> && ...),
                "EntityMngr::Attach: <Cmpts> isn't constructible");

  auto cmpts = AttachWithoutInit<Cmpts...>(e);

  return {new (std::get<Cmpts*>(cmpts)) Cmpts...};
}

template <typename Cmpt, typename... Args>
Cmpt* EntityMngr::AssignAttach(Entity e, Args&&... args) {
  static_assert(
      std::is_constructible_v<Cmpt, Args...>,
      "EntityMngr::AssignAttach: <Cmpt> isn't constructible with <Args...>");
  auto [cmpt] = AttachWithoutInit<Cmpt>(e);
  return new (cmpt) Cmpt{std::forward<Args>(args)...};
}

template <typename... Cmpts>
void EntityMngr::Detach(Entity e) {
  static_assert(sizeof...(Cmpts) > 0,
                "EntityMngr::Detach: sizeof...(<Cmpts>) > 0");
  static_assert(IsSet_v<TypeList<Entity, Cmpts...>>,
                "EntityMngr::Detach: <Cmpts> must be different");
  assert("EntityMngr::Detach: e is invalid" && Exist(e));

  auto& info = entityTable[e.Idx()];
  Archetype* srcArchetype = info.archetype;
  size_t srcIdxInArchetype = info.idxInArchetype;

  assert((srcArchetype->types.IsContain<Cmpts>() && ...));

  auto& srcCmptTypeSet = srcArchetype->GetCmptTypeSet();
  auto dstCmptTypeSet = srcCmptTypeSet;
  dstCmptTypeSet.Remove<Cmpts...>();
  size_t dstCmptTypeSetHashCode = dstCmptTypeSet.HashCode();

  // get dstArchetype
  Archetype* dstArchetype;
  auto target = h2a.find(dstCmptTypeSetHashCode);
  if (target == h2a.end()) {
    dstArchetype = Archetype::Remove<Cmpts...>(srcArchetype);
    assert(dstCmptTypeSet == dstArchetype->GetCmptTypeSet());
    h2a[dstCmptTypeSetHashCode] = dstArchetype;
    for (auto& [query, archetypes] : queryCache) {
      if (dstCmptTypeSet.IsMatch(query))
        archetypes.insert(dstArchetype);
    }
  } else
    dstArchetype = target->second;

  // move src to dst
  size_t dstIdxInArchetype = dstArchetype->RequestBuffer();
  auto srcCmptTraits = srcArchetype->GetRuntimeCmptTraits();
  for (auto type : srcCmptTypeSet) {
    auto [srcCmpt, srcSize] = srcArchetype->At(type, srcIdxInArchetype);
    if (dstCmptTypeSet.IsContain(type)) {
      auto [dstCmpt, dstSize] = dstArchetype->At(type, dstIdxInArchetype);
      assert(srcSize == dstSize);
      srcCmptTraits.MoveConstruct(type, dstCmpt, srcCmpt);
    } else
      srcCmptTraits.Destruct(type, srcCmpt);
  }

  // erase
  auto srcMovedEntityIndex = srcArchetype->Erase(srcIdxInArchetype);
  if (srcMovedEntityIndex != Entity::npos)
    entityTable[srcMovedEntityIndex].idxInArchetype = srcIdxInArchetype;

  info.archetype = dstArchetype;
  info.idxInArchetype = dstIdxInArchetype;
}

template <typename Cmpt>
bool EntityMngr::Have(Entity e) {
  static_assert(!std::is_same_v<Cmpt, Entity>,
                "EntityMngr::Have: <Cmpt> != Entity");
  assert(Exist(e));
  return entityTable[e.Idx()].archetype->GetCmptTypeSet().IsContain<Cmpt>();
}

template <typename Cmpt>
Cmpt* EntityMngr::Get(Entity e) {
  static_assert(!std::is_same_v<Cmpt, Entity>,
                "EntityMngr::Get: <Cmpt> != Entity");
  assert(Exist(e));
  const auto& info = entityTable[e.Idx()];
  return info.archetype->At<Cmpt>(info.idxInArchetype);
}
}  // namespace My
