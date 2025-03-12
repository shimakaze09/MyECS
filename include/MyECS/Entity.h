//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "detail/Util.h"

namespace My::MyECS {
// index + version
class Entity {
 public:
  size_t Idx() const noexcept { return idx; }

  size_t Version() const noexcept { return version; }

  friend bool operator==(const Entity& x, const Entity& y) noexcept {
    return x.idx == y.idx && x.version == y.version;
  }

  friend bool operator<(const Entity& x, const Entity& y) noexcept {
    return x.idx < y.idx || (x.idx == y.idx && x.version < y.version);
  }

  static constexpr Entity Invalid() noexcept {
    return {size_t_invalid, size_t_invalid};
  }

 private:
  friend class EntityMngr;
  friend class Archetype;

  constexpr Entity(size_t idx, size_t version) : idx(idx), version(version) {}

  size_t idx;
  size_t version;
};
}  // namespace My::MyECS
