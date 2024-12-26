//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "ArchetypeMngr.h"
#include "Entity.h"
#include "Pool.h"

#include <MyTemplate/FuncTraits.h>

#include <tuple>

namespace My::detail::World_ {
template <typename Args>
struct Each;
}

namespace My {
class World {
 public:
  World() : mngr(new ArchetypeMngr(this)) {}

  template <typename... Cmpts>
  inline Entity* CreateEntityWith();

  template <typename ArgList>
  friend struct detail::World_::Each;

  template <typename Sys>
  inline void Each(Sys&& s) {
    detail::World_::Each<typename FuncTraits<Sys>::ArgList>::run(
        this, std::forward<Sys>(s));
  }

 private:
  ArchetypeMngr* mngr;
};
}  // namespace My

#include "detail/World.inl"