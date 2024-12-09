#ifndef ARCHETYPE_MANAGER_HXX
#define ARCHETYPE_MANAGER_HXX

#include "Archetype.hxx"
#include "EntityBase.hxx"
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
  const std::vector<Archetype*> GetArchetypeWith();

  template <typename... Cmpts>
  const std::tuple<EntityBase*, Cmpts*...> CreateEntity();

  template <typename... Cmpts>
  const std::tuple<Cmpts*...> EntityAttach(EntityBase* e);

  template <typename... Cmpts>
  void EntityDetach(EntityBase* e);

  void Release(EntityBase* e);

 private:
  Pool<EntityBase> m_entityPool;
  std::map<std::pair<Archetype*, size_t>, EntityBase*>
      m_aiToEntity;  // (archetype, idx) -> entity
  std::set<Archetype::ID> m_ids;
  My::World* m_world;
  std::map<Archetype::ID, Archetype*> m_idToArchetype;  // id to archetype
};
}  // namespace My

#include "ArchetypeManager.inl"

#endif  // ARCHETYPE_MANAGER_HXX