//
// Created by Admin on 30/12/2024.
//

#pragma once

namespace My {
class Archetype;

struct EntityBase {
  Archetype* archetype{nullptr};
  size_t idx{static_cast<size_t>(-1)};

  bool operator<(const EntityBase& e) const noexcept {
    return archetype < e.archetype || (archetype == e.archetype && idx < e.idx);
  }

 private:
  friend class EntityMngr;
};
}  // namespace My