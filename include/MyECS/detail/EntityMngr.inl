//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "../CmptTag.h"

#include <MyTemplate/Func.h>
#include <MyTemplate/Typelist.h>

namespace My::MyECS {
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
std::tuple<Entity, Cmpts*...> EntityMngr::Create() {
  static_assert(IsSet_v<TypeList<Entity, Cmpts...>>,
                "EntityMngr::Create: <Cmpts> must be different");
  Archetype* archetype = GetOrCreateArchetypeOf<Cmpts...>();
  size_t entityIndex = RequestEntityFreeEntry();
  EntityInfo& info = entityTable[entityIndex];
  Entity e{entityIndex, info.version};
  info.archetype = archetype;
  auto [idxInArchetype, cmpts] = archetype->Create<Cmpts...>(e);
  info.idxInArchetype = idxInArchetype;
  return {e, std::get<Cmpts*>(cmpts)...};
}

template <typename... Cmpts>
void EntityMngr::AttachWithoutInit(Entity e) {
  assert(Exist(e));

  auto& info = entityTable[e.Idx()];
  Archetype* srcArchetype = info.archetype;
  size_t srcIdxInArchetype = info.idxInArchetype;

  const auto& srcCmptTypeSet = srcArchetype->GetCmptTypeSet();
  auto dstCmptTypeSet = srcCmptTypeSet;
  dstCmptTypeSet.Insert(CmptType::Of<Cmpts>...);
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

  if (dstArchetype == srcArchetype)
    return;

  // move src to dst
  size_t dstIdxInArchetype = dstArchetype->RequestBuffer();

  auto srcCmptTraits = srcArchetype->GetRTSCmptTraits();
  for (auto type : srcCmptTypeSet) {
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

template <typename... Cmpts>
std::tuple<Cmpts*...> EntityMngr::Attach(Entity e) {
  static_assert((std::is_constructible_v<Cmpts> && ...),
                "EntityMngr::Attach: <Cmpts> isn't default constructible");
  if (!Exist(e))
    throw std::invalid_argument("Entity is invalid");

  using CmptList = TypeList<Cmpts...>;
  const auto& cmptTypes = entityTable[e.Idx()].archetype->GetCmptTypeSet();
  std::array<bool, sizeof...(Cmpts)> needAttach = {
      !cmptTypes.Contains(CmptType::Of<Cmpts>)...};
  AttachWithoutInit<Cmpts...>(e);
  const auto& new_info = entityTable[e.Idx()];
  std::tuple<Cmpts*...> cmpts{
      new_info.archetype->At<Cmpts>(new_info.idxInArchetype)...};
  ((std::get<Find_v<CmptList, Cmpts>>(needAttach)
        ? new (std::get<Cmpts*>(cmpts)) Cmpts
        : nullptr),
   ...);

  return cmpts;
}

template <typename Cmpt, typename... Args>
Cmpt* EntityMngr::Emplace(Entity e, Args&&... args) {
  static_assert(std::is_constructible_v<Cmpt, Args...> ||
                    is_list_initializable_v<Cmpt, Args...>,
                "EntityMngr::Emplace: <Cmpt> isn't "
                "constructible/list_initializable with Args...");
  if (!Exist(e))
    throw std::invalid_argument("EntityMngr::Emplace: Entity is invalid");

  bool needAttach = !entityTable[e.Idx()].archetype->GetCmptTypeSet().Contains(
      CmptType::Of<Cmpt>);
  if (needAttach) {
    AttachWithoutInit<Cmpt>(e);
    const auto& info = entityTable[e.Idx()];
    Cmpt* cmpt = info.archetype->At<Cmpt>(info.idxInArchetype);
    return new (cmpt) Cmpt{std::forward<Args>(args)...};
  } else {
    const auto& info = entityTable[e.Idx()];
    return info.archetype->At<Cmpt>(info.idxInArchetype);
  }
}

inline bool EntityMngr::Have(Entity e, CmptType type) const {
  if (!Exist(e))
    throw std::invalid_argument("EntityMngr::Have: Entity is invalid");
  return entityTable[e.Idx()].archetype->GetCmptTypeSet().Contains(type);
}

template <typename Cmpt>
Cmpt* EntityMngr::Get(Entity e) const {
  static_assert(!std::is_same_v<Cmpt, Entity>,
                "EntityMngr::Get: <Cmpt> != Entity");
  if (!Exist(e))
    throw std::invalid_argument("EntityMngr::Get: Entity is invalid");
  const auto& info = entityTable[e.Idx()];
  return info.archetype->At<Cmpt>(info.idxInArchetype);
}

inline CmptPtr EntityMngr::Get(Entity e, CmptType type) const {
  if (!Exist(e))
    throw std::invalid_argument("EntityMngr::Get: Entity is invalid");
  const auto& info = entityTable[e.Idx()];
  return {type, info.archetype->At(type, info.idxInArchetype)};
}

inline bool EntityMngr::Exist(Entity e) const {
  return e.Idx() < entityTable.size() &&
         e.Version() == entityTable[e.Idx()].version;
}

inline void EntityMngr::AddCommand(const std::function<void()>& command) {
  std::lock_guard<std::mutex> guard(commandBufferMutex);
  commandBuffer.push_back(command);
}
}  // namespace My::MyECS
