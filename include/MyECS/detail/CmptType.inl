//
// Created by Admin on 4/03/2025.
//

#pragma once

namespace My {
constexpr bool operator<(CmptType x, CmptType y) noexcept {
  return x.HashCode() < y.HashCode();
}

constexpr bool operator==(CmptType x, CmptType y) noexcept {
  return x.HashCode() == y.HashCode();
}
}  // namespace My

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::CmptType> {
  constexpr size_t operator()(My::CmptType t) const noexcept {
    return t.HashCode();
  }
};
}  // namespace std
