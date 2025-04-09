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

  std::size_t GetValue() const noexcept {
    return hash_combine(filter.GetValue(), locator.GetValue());
  }

  bool operator==(const EntityQuery& query) const noexcept {
    return filter == query.filter && locator == query.locator;
  }
};
}  // namespace My::MyECS

#include "detail/EntityQuery.inl"
