//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "ArchetypeFilter.h"
#include "CmptLocator.h"
#include "detail/Util.h"

namespace My::MyECS {
// ArchetypeFilter + CmptLocator
struct EntityQuery {
  ArchetypeFilter filter;
  CmptLocator locator;

  size_t HashCode() const noexcept {
    return hash_combine(filter.HashCode(), locator.HashCode());
  }

  bool operator==(const EntityQuery& query) const noexcept {
    return filter == query.filter && locator == query.locator;
  }
};
}  // namespace My::MyECS

#include "detail/EntityQuery.inl"
