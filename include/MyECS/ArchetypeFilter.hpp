#pragma once

#include "AccessTypeID.hpp"

namespace Smkz::MyECS {
// filter Archetype with all, any and none
struct ArchetypeFilter {
  AccessTypeIDSet all;
  AccessTypeIDSet any;
  small_flat_set<TypeID> none;

  std::size_t GetValue() const noexcept;

  bool HaveWriteTypeID() const noexcept;

  bool operator==(const ArchetypeFilter& rhs) const noexcept;
};
}  // namespace Smkz::MyECS
