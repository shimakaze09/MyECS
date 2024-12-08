#ifndef ARCHETYPE_MANAGER_HXX
#define ARCHETYPE_MANAGER_HXX

#include "Archetype.hxx"
#include "EntityData.hxx"
#include "Pool.hxx"

#include <MyTemplate/TypeList.hxx>

namespace My {
class World;

class ArchetypeManager {
 public:
  ArchetypeManager(World* world) : m_world(world) {}

  ~ArchetypeManager() {
    for (auto p : m_idToArchetype)
      delete p.second;
  }

  inline World* World() const noexcept { return m_world; }

  inline Archetype* GetArchetypeOf(const Archetype::ID& archetypeID) {
    auto target = m_idToArchetype.find(archetypeID);
    assert(target != m_idToArchetype.end());
    return target->second;
  }

  template <typename... Cmpts>
  inline Archetype* GetOrCreateArchetypeOf();

  template <typename... Cmpts>
  const std::vector<Archetype*> LocateArchetypeWith();

  template <typename... Cmpts>
  EntityData* CreateEntity();

  template <typename Cmpt, typename... Args>
  Cmpt* EntityAttach(EntityData* e, Args&&... args);

  template <typename Cmpt>
  void EntityDetach(EntityData* e);

  void Release(EntityData* e);

 private:
  Pool<EntityData> m_entityPool;
  std::map<EntityData, EntityData*> m_dataToPointer;
  std::set<Archetype::ID> m_ids;
  My::World* m_world;
  std::map<Archetype::ID, Archetype*> m_idToArchetype;  // id to archetype
};
}  // namespace My

#include "detail/ArchetypeManager.inl"

#endif  // ARCHETYPE_MANAGER_HXX