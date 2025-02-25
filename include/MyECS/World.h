//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "detail/SystemMngr.h"

#include <MyTemplate/Func.h>

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
  std::tuple<Entity*, Cmpts*...> CreateEntity();

  void Update(bool dump = false);
  void RunCommand();

  template <typename Sys>
  void Each(Sys&& s);

  template <typename Sys>
  void Each(Sys&& s) const;

  template <typename Sys>
  void ParallelEach(Sys&& s);

  template <typename Sys>
  void ParallelEach(Sys&& s) const;

 private:
  SystemMngr sysMngr;
  tf::Executor executor;
  ArchetypeMngr mngr;

  template <typename ArgList>
  friend struct detail::World_::Each;
  template <typename ArgList>
  friend struct detail::World_::ParallelEach;
};
}  // namespace My

#include "detail/World.inl"