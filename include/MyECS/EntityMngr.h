//
// Created by Admin on 5/03/2025.
//

#pragma once

#include "EntityQuery.h"
#include "SingletonLocator.h"
#include "detail/Archetype.h"
#include "detail/Job.h"

namespace My::MyECS {
class World;
class SystemFunc;
class IListener;

// Entity Manager of World
// auto maintain Component's lifecycle ({default|copy|move} constructor, destructor)
// [API]
// - Entity: Create, Instantiate, Destroy, Exist
// - Component: Attach, Emplace, Detach, Have, Get, Components
// - Singleton: IsSingleton, GetSingletonEntity, GetSingleton
// - other: EntityNum, AddCommand
// [important]
// - some API with CmptType need RTDCmptTraits to get {size|alignment|lifecycle function} (throw std::logic_error)
// - API with Entity require Entity exist  (throw std::invalid_argument)
// [details]
// - when free entries is empty, use new entity entry (version is 0)
class EntityMngr {
 public:
  EntityMngr();
  EntityMngr(const EntityMngr& em);
  EntityMngr(EntityMngr&&) noexcept = default;
  ~EntityMngr();

  RTDCmptTraits cmptTraits;

  template <typename... Cmpts>
  std::tuple<Entity, Cmpts*...> Create();

  // use RTDCmptTraits
  Entity Create(Span<const CmptType> types);

  Entity Create(CmptType type) { return Create({&type, 1}); }

  Entity Instantiate(Entity);

  // TODO: CreateEntities

  template <typename... Cmpts>
  std::tuple<Cmpts*...> Attach(Entity);

  // use RTDCmptTraits
  void Attach(Entity, Span<const CmptType> types);

  void Attach(Entity e, CmptType type) { Attach(e, {&type, 1}); }

  template <typename Cmpt, typename... Args>
  Cmpt* Emplace(Entity, Args&&...);

  void Detach(Entity, Span<const CmptType> types);

  void Detach(Entity e, CmptType type) { Detach(e, {&type, 1}); }

  // use Detach(Entity, const CmptType*, size_t)
  template <typename... Cmpts>
  void Detach(Entity);

  bool Have(Entity, CmptType) const;
  // use Have(Entity, CmptType)
  template <typename Cmpt>
  bool Have(Entity) const;

  // nullptr if not containts CmptType
  CmptPtr Get(Entity, CmptType) const;
  // use Get(Entity, CmptType)
  template <typename Cmpt>
  Cmpt* Get(Entity) const;

  std::vector<CmptPtr> Components(Entity) const;

  bool Exist(Entity) const noexcept;

  void Destroy(Entity);

  size_t TotalEntityNum() const noexcept {
    return entityTable.size() - entityTableFreeEntry.size();
  }

  size_t EntityNum(const EntityQuery&) const;

  // use entry in reverse
  const std::vector<size_t>& GetEntityFreeEntries() const noexcept {
    return entityTableFreeEntry;
  }

  size_t GetEntityVersion(size_t idx) const noexcept {
    return entityTable.at(idx).version;
  }

  bool IsSingleton(CmptType) const;
  Entity GetSingletonEntity(CmptType) const;
  // nullptr if not singleton
  CmptPtr GetSingleton(CmptType) const;

  template <typename Cmpt>
  Cmpt* GetSingleton() const {
    return GetSingleton(CmptType::Of<Cmpt>).As<Cmpt>();
  }

  // filter's all contains cmpt
  template <typename Cmpt>
  std::vector<Cmpt*> GetCmptArray(const ArchetypeFilter&) const;
  std::vector<CmptPtr> GetCmptArray(const ArchetypeFilter&, CmptType) const;
  std::vector<Entity> GetEntityArray(const ArchetypeFilter&) const;

  void Accept(IListener* listener) const;

  EntityMngr& operator=(EntityMngr&&) noexcept = delete;
  EntityMngr& operator=(const EntityMngr&) = delete;

 private:
  friend class World;
  friend class Archetype;

  static bool IsSet(Span<const CmptType> types) noexcept;

  template <typename... Cmpts>
  Archetype* GetOrCreateArchetypeOf();
  // types not contain Entity
  Archetype* GetOrCreateArchetypeOf(Span<const CmptType> types);

  // return original archetype
  template <typename... Cmpts>
  Archetype* AttachWithoutInit(Entity);
  Archetype* AttachWithoutInit(Entity, Span<const CmptType> types);

  std::vector<CmptAccessPtr> LocateSingletons(const SingletonLocator&) const;

  const std::set<Archetype*>& QueryArchetypes(const EntityQuery&) const;
  mutable std::unordered_map<EntityQuery, std::set<Archetype*>> queryCache;

  // if job is nullptr, direct run
  void GenEntityJob(World*, Job*, SystemFunc*) const;
  // if job is nullptr, direct run
  void GenChunkJob(World*, Job*, SystemFunc*) const;
  // if job is nullptr, direct run
  void GenJob(World*, Job*, SystemFunc*) const;
  // if job is nullptr, direct run
  void AutoGen(World*, Job*, SystemFunc*) const;

  struct EntityInfo {
    Archetype* archetype{nullptr};
    size_t idxInArchetype{static_cast<size_t>(-1)};
    size_t version{0};  // version
  };

  std::vector<EntityInfo> entityTable;
  std::vector<size_t> entityTableFreeEntry;
  size_t RequestEntityFreeEntry();
  void RecycleEntityEntry(Entity);

  std::unique_ptr<Pool<Chunk>> sharedChunkPool;
  std::unordered_map<CmptTypeSet, std::unique_ptr<Archetype>>
      ts2a;  // archetype's CmptTypeSet to archetype
};
}  // namespace My::MyECS

#include "detail/EntityMngr.inl"
