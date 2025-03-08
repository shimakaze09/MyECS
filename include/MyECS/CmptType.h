//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <MyTemplate/TypeID.h>

namespace My {
// Component Type
// use a hashcode to distinguish different type
class CmptType {
 public:
  explicit constexpr CmptType(size_t id) : hashcode{id} {}

  explicit constexpr CmptType(std::string_view type_name)
      : hashcode{RuntimeTypeID(type_name)} {}

  template <typename Cmpt>
  static constexpr CmptType Of = CmptType{TypeID<Cmpt>};

  constexpr size_t HashCode() const noexcept { return hashcode; }

  static constexpr CmptType Invalid() noexcept {
    return CmptType{static_cast<size_t>(-1)};
  }

  template <typename Cmpt>
  bool Is() const noexcept {
    return hashcode == TypeID<Cmpt>;
  }

  bool operator<(const CmptType& rhs) const noexcept {
    return hashcode < rhs.hashcode;
  }

  bool operator==(const CmptType& rhs) const noexcept {
    return hashcode == rhs.hashcode;
  }

  bool operator!=(const CmptType& rhs) const noexcept {
    return hashcode != rhs.hashcode;
  }

 private:
  size_t hashcode;
};
}  // namespace My

#include "detail/CmptType.inl"
