//
// Created by Admin on 4/03/2025.
//

#pragma once

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
