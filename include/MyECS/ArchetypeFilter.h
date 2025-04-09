//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "AccessTypeID.h"

#include <set>

namespace My::MyECS {
// filter Archetype with all, any and none
struct ArchetypeFilter {
  AccessTypeIDSet all;
  AccessTypeIDSet any;
  std::set<TypeID> none;

  std::size_t GetValue() const noexcept;

  bool HaveWriteTypeID() const noexcept;

  bool operator==(const ArchetypeFilter& rhs) const noexcept;
};
}  // namespace My::MyECS
