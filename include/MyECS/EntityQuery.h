//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "ArchetypeFilter.h"
#include "CmptLocator.h"
#include "detail/Util.h"

namespace My::MyECS {
// ArchetypeFilter + CmptLocator
class EntityQuery {
 public:
  ArchetypeFilter filter;
  CmptLocator locator;

  template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts,
            typename... Cmpts>
  EntityQuery(TypeList<AllCmpts...>, TypeList<AnyCmpts...>,
              TypeList<NoneCmpts...>, TypeList<Cmpts...>);

  EntityQuery(ArchetypeFilter filter = {}, CmptLocator locator = {})
      : filter{std::move(filter)}, locator{std::move(locator)} {}

  size_t HashCode() const noexcept {
    return hash_combine(filter.HashCode(), locator.HashCode());
  }

  bool operator==(const EntityQuery& query) const {
    return filter == query.filter && locator == query.locator;
  }
};
}  // namespace My::MyECS

#include "detail/EntityQuery.inl"
