#ifndef WORLD_HXX
#define WORLD_HXX

#include "Archetype.hxx"
#include "ArchetypeManager.hxx"
#include "Entity.hxx"
#include "Pool.hxx"

#include <MyTemplate/FuncTraits.hxx>

#include <tuple>

namespace My::detail::World_ {
template <typename Arg>
struct Each;
}

namespace My {
class World {
 public:
  World() : m_manager(this) {}

  template <typename... Cmpts>
  inline Entity* CreateEntityWith();

  void Delete(Entity* entity);

  template <typename ArgList>
  friend struct detail::World_::Each;

  template <typename Sys>
  inline void Each(Sys&& s) {
    detail::World_::Each<typename FuncTraits<Sys>::ArgList>::Run(
        this, std::forward<Sys>(s));
  }

 private:
  detail::ArchetypeManager m_manager;
  Pool<Entity> m_entities;
  std::map<Entity*, std::tuple<detail::Archetype*, size_t>> m_entityToAtid;
  std::map<std::tuple<detail::Archetype*, size_t>, Entity*> m_atidToEntity;
};
}  // namespace My

#include "detail/World.inl"

#endif  // WORLD_HXX