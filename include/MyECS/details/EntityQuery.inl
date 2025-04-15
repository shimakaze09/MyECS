#pragma once

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::MyECS::EntityQuery> {
  std::size_t operator()(const My::MyECS::EntityQuery& query) const noexcept {
    return query.GetValue();
  }
};
}  // namespace std
