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
template <typename Args>
struct ParallelEach;
}  // namespace My::detail::World_

namespace My {
class World {
 public:
  World() : m_manager(new ArchetypeManager(this)) {}

  template <typename... Cmpts>
  inline Entity* CreateEntity();

  // s must be a callable object, and it's argument-list isn't empty
  template <typename ArgList>
  friend struct detail::World_::Each;

  template <typename Sys>
  inline void Each(Sys&& s) {
    detail::World_::Each<typename FuncTraits<Sys>::ArgList>::Run(
        this, std::forward<Sys>(s));
  }

  // s must be a callable object, and it's argument-list isn't empty
  template <typename ArgList>
  friend struct detail::World_::ParallelEach;

  template <typename Sys>
  inline void ParallelEach(Sys&& s) {
    detail::World_::ParallelEach<typename FuncTraits<Sys>::ArgList>::run(
        this, std::forward<Sys>(s));
  }

 private:
  ArchetypeManager* m_manager;
};
}  // namespace My

#include "detail/World.inl"

#endif  // WORLD_HXX