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

  static constexpr Entity Invalid() noexcept {
    return {size_t_invalid, size_t_invalid};
  }

  constexpr bool IsValid() const noexcept { return idx == size_t_invalid; }

  bool operator==(const Entity& rhs) const noexcept {
    return idx == rhs.idx && version == rhs.version;
  }

  bool operator<(const Entity& rhs) const noexcept {
    return idx < rhs.idx || (idx == rhs.idx && version < rhs.version);
  }

 private:
  friend class EntityMngr;
  friend class Archetype;

  constexpr Entity(size_t idx, size_t version) noexcept
      : idx(idx), version(version) {}

  size_t idx;
  size_t version;
};
}  // namespace My::MyECS
