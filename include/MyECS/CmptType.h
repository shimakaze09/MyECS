//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <MyTemplate/TypeID.h>

namespace My {
class CmptType {
 public:
  explicit constexpr CmptType(size_t id) : hashcode{id} {}

  template <typename Cmpt>
  static constexpr CmptType Of() noexcept {
    return CmptType{TypeID<Cmpt>};
  }

  constexpr size_t HashCode() const noexcept { return hashcode; }

  template <typename Cmpt>
  static constexpr size_t HashCodeOf() noexcept {
    return TypeID<Cmpt>;
  }

  template <typename Cmpt>
  bool Is() const noexcept {
    return hashcode == HashCodeOf<Cmpt>();
  }

 private:
  size_t hashcode;
};
}  // namespace My

#include "detail/CmptType.inl"
