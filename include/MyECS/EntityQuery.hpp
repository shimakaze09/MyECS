#pragma once

#include "ArchetypeFilter.hpp"
#include "CmptLocator.hpp"
#include "details/Util.hpp"

namespace My::MyECS {
// ArchetypeFilter + CmptLocator
struct EntityQuery {
  ArchetypeFilter filter;
  CmptLocator locator;

  std::size_t GetValue() const noexcept {
    return hash_combine(filter.GetValue(), locator.GetValue());
  }

  bool operator==(const EntityQuery& query) const noexcept {
    return filter == query.filter && locator == query.locator;
  }
};
}  // namespace My::MyECS

template <>
struct std::hash<My::MyECS::EntityQuery> {
  std::size_t operator()(const My::MyECS::EntityQuery& query) const noexcept {
    return query.GetValue();
  }
};
