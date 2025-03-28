//
// Created by Admin on 4/03/2025.
//

#pragma once

namespace std {
template <typename T>
struct hash;
}  // namespace std

template <>
struct std::hash<My::MyECS::CmptType> {
  constexpr size_t operator()(const My::MyECS::CmptType& t) const noexcept {
    return t.HashCode();
  }
};

template <>
struct std::hash<My::MyECS::CmptAccessType> {
  constexpr size_t operator()(
      const My::MyECS::CmptAccessType& t) const noexcept {
    return t.HashCode();
  }
};
