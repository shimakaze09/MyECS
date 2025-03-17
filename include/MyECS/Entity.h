//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "detail/Util.h"

namespace My::MyECS {
// index + version
class Entity {
 public:
  constexpr Entity(size_t idx, size_t version) noexcept
      : idx(idx), version(version) {}

  constexpr Entity() noexcept : Entity{Invalid()} {}

  constexpr size_t Idx() const noexcept { return idx; }

  constexpr size_t Version() const noexcept { return version; }

  static constexpr Entity Invalid() noexcept {
    return {size_t_invalid, size_t_invalid};
  }

  constexpr bool IsValid() const noexcept { return idx == size_t_invalid; }

  constexpr bool operator==(const Entity& rhs) const noexcept {
    return idx == rhs.idx && version == rhs.version;
  }

  constexpr bool operator<(const Entity& rhs) const noexcept {
    return idx < rhs.idx || (idx == rhs.idx && version < rhs.version);
  }

 private:
  friend class EntityMngr;

  size_t idx;
  size_t version;
};
}  // namespace My::MyECS
