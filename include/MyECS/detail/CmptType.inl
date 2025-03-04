//
// Created by Admin on 4/03/2025.
//

#pragma once

namespace My {
inline constexpr bool operator<(const CmptType& x, const CmptType& y) noexcept {
  return x.HashCode() < y.HashCode();
}

inline constexpr bool operator==(const CmptType& x,
                                 const CmptType& y) noexcept {
  return x.HashCode() == y.HashCode();
}
}  // namespace My

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::CmptType> {
  constexpr size_t operator()(const My::CmptType& t) const noexcept {
    return t.HashCode();
  }
};
}  // namespace std
