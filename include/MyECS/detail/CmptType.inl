//
// Created by Admin on 4/03/2025.
//

#pragma once

namespace My::MyECS {
template <typename Cmpt>
constexpr bool CmptType::Is() const noexcept {
  static_assert(!IsTaggedCmpt_v<Cmpt>);
  return hashcode == TypeID<Cmpt>;
}
}  // namespace My::MyECS

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::MyECS::CmptType> {
  constexpr size_t operator()(const My::MyECS::CmptType& t) const noexcept {
    return t.HashCode();
  }
};

template <>
struct hash<My::MyECS::CmptAccessType> {
  constexpr size_t operator()(
      const My::MyECS::CmptAccessType& t) const noexcept {
    return t.HashCode();
  }
};
}  // namespace std
