//
// Created by Admin on 4/03/2025.
//

#pragma once

namespace My {
template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts,
          typename... Cmpts>
EntityQuery::EntityQuery(TypeList<AllCmpts...>, TypeList<AnyCmpts...>,
                         TypeList<NoneCmpts...>, TypeList<Cmpts...>)
    : filter{TypeList<AllCmpts...>{}, TypeList<AnyCmpts...>{},
             TypeList<NoneCmpts...>{}},
      locator{TypeList<Cmpts...>{}} {}

inline bool operator<(const EntityQuery& x, const EntityQuery& y) noexcept {
  return x.HashCode() < y.HashCode();
}

inline bool operator==(const EntityQuery& x, const EntityQuery& y) noexcept {
  return x.HashCode() == y.HashCode();
}
}  // namespace My

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::EntityQuery> {
  size_t operator()(const My::EntityQuery& query) const noexcept {
    return query.HashCode();
  }
};
}  // namespace std
