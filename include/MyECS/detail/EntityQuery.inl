//
// Created by Admin on 4/03/2025.
//

#pragma once

namespace My::MyECS {
template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts,
          typename... Cmpts>
EntityQuery::EntityQuery(TypeList<AllCmpts...>, TypeList<AnyCmpts...>,
                         TypeList<NoneCmpts...>, TypeList<Cmpts...>)
    : filter{TypeList<AllCmpts...>{}, TypeList<AnyCmpts...>{},
             TypeList<NoneCmpts...>{}},
      locator{TypeList<Cmpts...>{}} {}
}  // namespace My::MyECS

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::MyECS::EntityQuery> {
  size_t operator()(const My::MyECS::EntityQuery& query) const noexcept {
    return query.HashCode();
  }
};
}  // namespace std
