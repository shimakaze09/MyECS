//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Archetype.h"
#include "EntityData.h"
#include "Pool.h"

#include <MyTemplate/TypeList.h>

namespace My {
class World;

class ArchetypeMngr {
 public:
  ArchetypeMngr(World* w) : w(w) {}

  ~ArchetypeMngr() {
    for (auto p : id2a)
      delete p.second;
  }

  inline World* World() const noexcept { return w; }

  inline Archetype* GetArchetypeOf(const Archetype::ID& archetypeID) {
    auto target = id2a.find(archetypeID);
    assert(target != id2a.end());
    return target->second;
  }

  template <typename... Cmpts>
  inline Archetype* GetOrCreateArchetypeOf();

  template <typename... Cmpts>
  const std::vector<Archetype*> LocateArchetypeWith();

  template <typename... Cmpts>
  EntityData* CreateEntity();

  template <typename Cmpt, typename... Args>
  Cmpt* EntityAdd(EntityData* e, Args&&... args);

  void Release(EntityData* e);

 private:
  Pool<EntityData> entityPool;
  std::map<EntityData, EntityData*> d2p;
  std::set<Archetype::ID> ids;
  My::World* w;
  std::map<Archetype::ID, Archetype*> id2a;  // id to archetype
};
}  // namespace My

#include "detail/ArchetypeMngr.inl"