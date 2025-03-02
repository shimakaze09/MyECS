//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Archetype.h"
#include "EntityBase.h"

#include "Job.h"

#include <MyBL/Pool.h>

#include <mutex>

namespace My {
class World;

class ArchetypeMngr {
 public:
  ArchetypeMngr(World* w) : w{w} {}

  ~ArchetypeMngr();

  World* World() const noexcept { return w; }

  inline Archetype* GetArchetypeOf(const CmptIDSet& archetypeID);

  template <typename... Cmpts>
  inline Archetype* GetOrCreateArchetypeOf();

  template <typename... Cmpts>
  const std::set<Archetype*>& GetArchetypeWith();

  template <typename... Cmpts>
  const std::tuple<EntityBase*, Cmpts*...> CreateEntity();

  // TODO: CreateEntities
  template <typename... Cmpts>
  const std::tuple<Cmpts*...> EntityAttach(EntityBase* e);

  template <typename... Cmpts>
  void EntityDetach(EntityBase* e);

  void Release(EntityBase* e);

  template <typename Sys>
  void GenJob(Job* job, Sys&& sys);

  void AddCommand(const std::function<void()>& command);
  void RunCommands();

 private:
  My::World* w;

  Pool<EntityBase> entityPool;

  std::map<std::tuple<Archetype*, size_t>, EntityBase*>
      ai2e;  // (archetype, idx) -> entity

  std::set<CmptIDSet> ids;
  std::map<CmptIDSet, Archetype*> id2a;  // id to archetype

  // Query Cache
  // TypeID<Typelist<Cmpts...>> to archetype set
  // Typelist<Cmpts...> is sorted
  std::unordered_map<size_t, std::set<Archetype*>> cmpts2as;
  // TypeID<Typelist<Cmpts...>> to Cmpt ID set
  // Typelist<Cmpts...> is sorted
  std::unordered_map<size_t, CmptIDSet> cmpts2ids;

  std::vector<std::function<void()>> commandBuffer;
  std::mutex commandBufferMutex;
};
}  // namespace My

#include "ArchetypeMngr.inl"
