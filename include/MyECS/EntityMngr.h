//
// Created by Admin on 5/03/2025.
//

#pragma once

#include "EntityQuery.h"
#include "SystemFunc.h"
#include "detail/Archetype.h"
#include "detail/Job.h"

#include <MyContainer/Pool.h>

namespace My {
class World;

class EntityMngr {
 public:
  template <typename... Cmpts>
  const std::tuple<Entity, Cmpts*...> CreateEntity();

  // TODO: CreateEntities

  template <typename... Cmpts>
  const std::tuple<Cmpts*...> Attach(Entity e);

  template <typename Cmpt, typename... Args>
  Cmpt* AssignAttach(Entity e, Args&&... args);

  template <typename... Cmpts>
  void Detach(Entity e);

  void Destroy(Entity e);

  bool Exist(Entity e);

  template <typename Cmpt>
  bool Have(Entity e);
  template <typename Cmpt>
  Cmpt* Get(Entity e);

  size_t EntityNum(const EntityQuery& query) const;

 private:
  friend class World;
  EntityMngr() = default;
  ~EntityMngr();

  const std::set<Archetype*>& QueryArchetypes(const EntityQuery& query) const;

  template <typename... Cmpts>
  Archetype* GetOrCreateArchetypeOf();

  template <typename... Cmpts>
  const std::tuple<Cmpts*...> AttachWithoutInit(Entity e);

  void GenJob(Job* job, SystemFunc* sys) const;

  struct EntityInfo {
    Archetype* archetype{nullptr};
    size_t idxInArchetype{Archetype::npos};
    size_t version{0};  // version
  };

  std::vector<EntityInfo> entityTable;
  std::vector<size_t> entityTableFreeEntry;
  size_t RequestEntityFreeEntry();
  void RecycleEntityEntry(Entity e);

  std::unordered_map<size_t, Archetype*>
      h2a;  // archetype's hashcode to archetype

  mutable std::unordered_map<EntityQuery, std::set<Archetype*>> queryCache;
};
}  // namespace My

#include "detail/EntityMngr.inl"
