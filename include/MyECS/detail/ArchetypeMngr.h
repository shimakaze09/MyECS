//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Archetype.h"
#include "EntityBase.h"
#include "Pool.h"

#include <MyTemplate/TypeList.h>

#include <taskflow/taskflow.hpp>

namespace My {
class World;

class ArchetypeMngr {
 public:
  ArchetypeMngr(SystemMngr* sysmngr, World* w) : sysmngr{sysmngr}, w{w} {}

  ~ArchetypeMngr();

  inline World* World() const noexcept { return w; }

  inline Archetype* GetArchetypeOf(const Archetype::ID& archetypeID);

  template <typename... Cmpts>
  inline Archetype* GetOrCreateArchetypeOf();

  template <typename... Cmpts>
  const std::vector<Archetype*> GetArchetypeWith();

  template <typename... Cmpts>
  const std::tuple<EntityBase*, Cmpts*...> CreateEntity();

  template <typename... Cmpts>
  const std::tuple<Cmpts*...> EntityAttach(EntityBase* e);

  template <typename... Cmpts>
  void EntityDetach(EntityBase* e);

  void Release(EntityBase* e);

  template <typename Sys>
  void GenTaskflow(tf::Taskflow& taskflow, Sys&& sys);

 private:
  Pool<EntityBase> entityPool;
  std::map<std::tuple<Archetype*, size_t>, EntityBase*>
      ai2e;  // (archetype, idx) -> entity

  std::set<Archetype::ID> ids;
  std::map<Archetype::ID, Archetype*> id2a;  // id to archetype

  My::World* w;
  SystemMngr* sysmngr;
};
}  // namespace My

#include "ArchetypeMngr.inl"