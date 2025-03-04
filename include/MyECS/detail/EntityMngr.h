//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Archetype.h"
#include "EntityData.h"
#include "Job.h"

#include <MyContainer/Pool.h>

#include <mutex>

namespace My {
class World;

class EntityMngr {
 public:
  EntityMngr(World* w) : w{w} {}

  ~EntityMngr();

  World* World() const noexcept { return w; }

  template <typename... Cmpts>
  inline Archetype* GetOrCreateArchetypeOf();

  template <typename AllList, typename AnyList, typename NoneList,
            typename LocateList>
  const std::set<Archetype*>& QueryArchetypes() const;

  template <typename... Cmpts>
  const std::tuple<EntityData*, Cmpts*...> CreateEntity();

  // TODO: CreateEntities

  template <typename... Cmpts>
  const std::tuple<Cmpts*...> EntityAttach(EntityData* e);

  template <typename Cmpt, typename... Args>
  Cmpt* EntityAssignAttach(EntityData* e, Args... args);

  template <typename... Cmpts>
  void EntityDetach(EntityData* e);

  void Release(EntityData* e);

  template <typename Sys>
  void GenJob(Job* job, Sys&& sys) const;

  void AddCommand(const std::function<void()>& command);
  void RunCommands();

 private:
  template <typename... Cmpts>
  const std::tuple<Cmpts*...> EntityAttachWithoutInit(EntityData* e);

  template <typename... Cmpts>
  static std::vector<size_t> TypeListToIDVec(TypeList<Cmpts...>);

  My::World* w;

  Pool<EntityData> entityPool;

  std::map<std::tuple<Archetype*, size_t>, EntityData*>
      ai2e;  // (archetype, idx) -> entity

  std::unordered_map<size_t, Archetype*> h2a;  // CmptIDSet's hash to archetype

  // Query Cache
  // TypeID<AllList, AnyList, NoneList, LocateList> to archetype set
  // AllList, AnyList, NoneList, LocateList are **sorted**
  mutable std::unordered_map<size_t, std::set<Archetype*>> queryCache;

  struct Query {
    Query(const std::vector<size_t>& allCmptIDs,
          const std::vector<size_t>& anyCmptIDs,
          const std::vector<size_t>& noneCmptIDs,
          const std::vector<size_t>& locateCmptIDs)
        : allCmptIDs{allCmptIDs},
          anyCmptIDs{anyCmptIDs},
          noneCmptIDs{noneCmptIDs},
          locateCmptIDs{locateCmptIDs} {}

    std::vector<size_t> allCmptIDs;     // sorted
    std::vector<size_t> anyCmptIDs;     // sorted
    std::vector<size_t> noneCmptIDs;    // sorted
    std::vector<size_t> locateCmptIDs;  // sorted
  };

  mutable std::unordered_map<size_t, Query> id2query;

  // command
  std::vector<std::function<void()>> commandBuffer;
  std::mutex commandBufferMutex;
};
}  // namespace My

#include "EntityMngr.inl"
