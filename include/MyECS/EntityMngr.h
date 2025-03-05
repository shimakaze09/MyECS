//
// Created by Admin on 5/03/2025.
//

#pragma once

#include "EntityQuery.h"
#include "SystemFunc.h"
#include "detail/Archetype.h"
#include "detail/Job.h"

#include <MyContainer/Pool.h>

#include <mutex>

namespace My {
class World;

class EntityMngr {
 public:
  template <typename... Cmpts>
  std::tuple<Entity, Cmpts*...> CreateEntity();
  // use RTDCmptTraits
  Entity CreateEntity(const CmptType* types, size_t num);
  // call CreateEntity(const CmptType*, size_t)
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  Entity CreateEntity(CmptTypes...);

  Entity Instantiate(Entity);

  // TODO: CreateEntities

  template <typename... Cmpts>
  std::tuple<Cmpts*...> Attach(Entity);
  // use RTDCmptTraits
  void Attach(Entity, const CmptType* types, size_t num);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  std::array<CmptPtr, sizeof...(CmptTypes)> Attach(Entity, CmptTypes...);

  template <typename Cmpt, typename... Args>
  Cmpt* Emplace(Entity, Args&&...);

  template <typename... Cmpts>
  void Detach(Entity);
  // use RTDCmptTraits
  void Detach(Entity, const CmptType* types, size_t num);
  // call Detach(Entity, const CmptType*, size_t);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void Detach(Entity, CmptTypes...);

  template <typename Cmpt>
  bool Have(Entity) const;
  inline bool Have(Entity, CmptType) const;

  template <typename Cmpt>
  Cmpt* Get(Entity) const;
  inline CmptPtr Get(Entity, CmptType) const;

  std::vector<CmptPtr> Components(Entity) const;

  bool Exist(Entity) const;

  void Destroy(Entity);

  size_t EntityNum(const EntityQuery&) const;

  void AddCommand(const std::function<void()>& command);

 private:
  friend class World;
  EntityMngr() = default;
  ~EntityMngr();

  const std::set<Archetype*>& QueryArchetypes(const EntityQuery& query) const;

  template <typename... Cmpts>
  Archetype* GetOrCreateArchetypeOf();
  Archetype* GetOrCreateArchetypeOf(const CmptType* types, size_t num);
  // call GetOrCreateArchetypeOf(const CmptType*, size_t)
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  Archetype* GetOrCreateArchetypeOf(CmptTypes...);

  void AttachWithoutInit(Entity, const CmptType* types, size_t num);
  // call AttachWithoutInit(Entity, const CmptType*, size_t)
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void AttachWithoutInit(Entity, CmptTypes...);

  void GenJob(Job* job, SystemFunc* sys) const;

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

  // command
  std::vector<std::function<void()>> commandBuffer;
  std::mutex commandBufferMutex;
  void RunCommands();
};
}  // namespace My

#include "detail/EntityMngr.inl"
