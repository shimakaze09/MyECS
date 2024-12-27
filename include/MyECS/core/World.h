//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"

#include <MyTemplate/FuncTraits.h>

#include <thread>

namespace My::detail::World_ {
template <typename Args>
struct Each;
template <typename Args>
struct ParallelEach;
}  // namespace My::detail::World_

// no any lock !
namespace My {
class World {
 public:
  World();
  ~World();

  template <typename... Cmpts>
  inline std::tuple<Entity*, Cmpts*...> CreateEntity();

  // s must be a callable object and it's argument-list isn't empty
  template <typename Sys>
  inline void Each(Sys&& s);

  // s must be a callable object and it's argument-list isn't empty
  template <typename Sys>
  inline void ParallelEach(Sys&& s);

 private:
  template <typename ArgList>
  friend struct detail::World_::Each;
  template <typename ArgList>
  friend struct detail::World_::ParallelEach;

  ArchetypeMngr* mngr;
};
}  // namespace My

#include "detail/World.inl"