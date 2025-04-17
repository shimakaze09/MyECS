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

  bool IsMatch(const small_flat_set<TypeID>& types) const noexcept {
    return std::find_if_not(filter.all.begin(), filter.all.end(),
                            [&](const auto& type) {
                              return types.contains(type);
                            }) == filter.all.end() &&
           (filter.any.empty() ||
            std::find_if(filter.any.begin(), filter.any.end(),
                         [&](const auto& type) {
                           return types.contains(type);
                         }) != filter.any.end()) &&
           std::find_if(filter.none.begin(), filter.none.end(),
                        [&](const auto& type) {
                          return types.contains(type);
                        }) == filter.none.end();
  }

  friend bool operator==(const EntityQuery& lhs,
                         const EntityQuery& rhs) noexcept {
    return lhs.filter == rhs.filter && lhs.locator == rhs.locator;
  }
};
}  // namespace My::MyECS

template <>
struct std::hash<My::MyECS::EntityQuery> {
  std::size_t operator()(const My::MyECS::EntityQuery& query) const noexcept {
    return query.GetValue();
  }
};
