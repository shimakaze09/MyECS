#ifndef WORLD_HXX
#define WORLD_HXX


#include "Entity.hxx"

#include <MyTemplate/FuncTraits.hxx>

#include <thread>

namespace My::detail::World_ {
template <typename Arg>
struct Each;
template <typename Args>
struct ParallelEach;
}  // namespace My::detail::World_

namespace My {
class World {
 public:
  World();
  ~World();

  template <typename... Cmpts>
  inline std::tuple<Entity*, Cmpts*...> CreateEntity();

  // s must be a callable object, and it's argument-list isn't empty
  template <typename Sys>
  inline void Each(Sys&& s);

  // s must be a callable object, and it's argument-list isn't empty
  template <typename Sys>
  inline void ParallelEach(Sys&& s);

 private:
  template <typename ArgList>
  friend struct detail::World_::Each;
  template <typename ArgList>
  friend struct detail::World_::ParallelEach;

  ArchetypeManager* m_manager;
};
}  // namespace My

#include "detail/World.inl"

#endif  // WORLD_HXX