//
// Created by Admin on 5/03/2025.
//

#pragma once

#include <MyContainer/Pool.h>

#include "EntityQuery.h"
#include "SystemFunc.h"
#include "detail/Archetype.h"
#include "detail/Job.h"

namespace My::MyECS {
class World;

class IListener;

// Entity Manager of World
// auto maintain Component's lifecycle ({default|copy|move} constructor,
// destructor) [API]
// - Entity: Create, Instantiate, Destroy, Exist
// - Component: Attach, Emplace, Detach, Have, Get, Components
// - other: EntityNum, AddCommand
// [important]
// - API with CmptType need RTDCmptTraits to get {size|alignment|lifecycle
// function} (throw std::logic_error)
// - API with Entity require Entity exist  (throw std::invalid_argument)
class EntityMngr {
 public:
  template <typename... Cmpts>
  std::tuple<Entity, Cmpts*...> Create();

  // use RTDCmptTraits
  Entity Create(const CmptType* types, size_t num);

  Entity Instantiate(Entity);

  // TODO: CreateEntities

  template <typename... Cmpts>
  std::tuple<Cmpts*...> Attach(Entity);

  // use RTDCmptTraits
  void Attach(Entity, const CmptType* types, size_t num);

  template <typename Cmpt, typename... Args>
  Cmpt* Emplace(Entity, Args&&...);

  // use RTDCmptTraits
  void Detach(Entity, const CmptType* types, size_t num);

  bool Have(Entity, CmptType) const;

  template <typename Cmpt>
  Cmpt* Get(Entity) const;
  CmptPtr Get(Entity, CmptType) const;

  std::vector<CmptPtr> Components(Entity) const;

  bool Exist(Entity) const;

  void Destroy(Entity);

  size_t EntityNum(const EntityQuery&) const;

  bool IsSingleton(CmptType) const;
  Entity GetSingletonEntity(CmptType) const;
  CmptPtr GetSingleton(CmptType) const;

  template <typename Cmpt>
  Cmpt* GetSingleton() const {
    return GetSingleton(CmptType::Of<Cmpt>).As<Cmpt>();
  }

  void Accept(IListener* listener) const;

 private:
  friend class World;
  EntityMngr() = default;
  ~EntityMngr();

  static bool IsSet(const CmptType* types, size_t num);

  const std::set<Archetype*>& QueryArchetypes(const EntityQuery& query) const;

  template <typename... Cmpts>
  Archetype* GetOrCreateArchetypeOf();
  Archetype* GetOrCreateArchetypeOf(const CmptType* types, size_t num);

  template <typename... Cmpts>
  void AttachWithoutInit(Entity);
  void AttachWithoutInit(Entity, const CmptType* types, size_t num);

  void GenEntityJob(World*, Job*, SystemFunc*) const;
  void GenChunkJob(World*, Job*, SystemFunc*) const;

  struct EntityInfo {
    Archetype* archetype{nullptr};
    size_t idxInArchetype{size_t_invalid};
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
}  // namespace My::MyECS

#include "detail/EntityMngr.inl"
