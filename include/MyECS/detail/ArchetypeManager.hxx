#ifndef ARCHETYPE_MANAGER_HXX
#define ARCHETYPE_MANAGER_HXX

#include "Archetype.hxx"
#include "EntityBase.hxx"
#include "Pool.hxx"

#include <MyTemplate/TypeList.hxx>

namespace My {
class World;

class ArchetypeMngr {
 public:
  ArchetypeMngr(World* w) : w(w) {}

  ~ArchetypeMngr();

  inline World* World() const noexcept { return w; }

  inline Archetype* GetArchetypeOf(const Archetype::ID& archetypeID) {
    auto target = id2a.find(archetypeID);
    assert(target != id2a.end());
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
  Pool<EntityBase> entityPool;
  std::map<std::tuple<Archetype*, size_t>, EntityBase*>
      ai2e;  // (archetype, idx) -> entity
  std::set<Archetype::ID> ids;
  Ubpa::World* w;
  std::map<Archetype::ID, Archetype*> id2a;  // id to archetype
};
}  // namespace My

#include "ArchetypeManager.inl"

#endif  // ARCHETYPE_MANAGER_HXX