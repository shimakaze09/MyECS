//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "detail/SystemMngr.h"

#include <MyTemplate/Func.h>

#include <thread>

namespace My::detail::World_ {
template <typename Args>
struct Each;
template <typename Args>
struct ParallelEach;
}  // namespace My::detail::World_

namespace My {
class World {
 public:
  World();

  template <typename... Cmpts>
  inline std::tuple<Entity*, Cmpts*...> CreateEntity();

  void Update(bool dump = false);
  void RunCommand();

  template <typename Sys>
  inline void Each(Sys&& s);

  template <typename Sys>
  inline void ParallelEach(Sys&& s);

 private:
  template <typename ArgList>
  friend struct detail::World_::Each;
  template <typename ArgList>
  friend struct detail::World_::ParallelEach;

  SystemMngr sysMngr;
  tf::Executor executor;
  ArchetypeMngr mngr;
};
}  // namespace My

#include "detail/World.inl"