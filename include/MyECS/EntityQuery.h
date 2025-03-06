//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "EntityFilter.h"
#include "EntityLocator.h"

namespace My {
// EntityFilter + EntityLocator
class EntityQuery {
 public:
  EntityFilter filter;
  EntityLocator locator;

  template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts,
            typename... Cmpts>
  EntityQuery(TypeList<AllCmpts...>, TypeList<AnyCmpts...>,
              TypeList<NoneCmpts...>, TypeList<Cmpts...>);

  EntityQuery(EntityFilter filter, EntityLocator locator)
      : filter{std::move(filter)}, locator{std::move(locator)} {}

  size_t HashCode() const noexcept {
    return hash_combine(filter.HashCode(), locator.HashCode());
  }

  bool operator==(const EntityQuery& query) const noexcept {
    return filter == query.filter && locator == query.locator;
  }
};
}  // namespace My

#include "detail/EntityQuery.inl"
