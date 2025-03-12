//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptTag.h"

#include <MyTemplate/TypeID.h>

namespace My::MyECS {
// Component Type
// use a hashcode to distinguish different type
class CmptType {
 public:
  explicit constexpr CmptType(size_t id, AccessMode mode = AccessMode::WRITE)
      : hashcode{id}, mode{mode} {}

  explicit constexpr CmptType(std::string_view type_name,
                              AccessMode mode = AccessMode::WRITE)
      : hashcode{RuntimeTypeID(type_name)}, mode{mode} {}

  template <
      typename TaggedCmpt>  // non-tagged component's access mode is AccessMode::WRITE
  static constexpr CmptType Of =
      CmptType{TypeID<RemoveTag_t<TaggedCmpt>>, AccessModeOf<TaggedCmpt>};

  constexpr size_t HashCode() const noexcept { return hashcode; }

  constexpr AccessMode GetAccessMode() const noexcept { return mode; }

  static constexpr CmptType Invalid() noexcept {
    return CmptType{static_cast<size_t>(-1)};
  }

  template <typename Cmpt>  // non-tagged
  constexpr bool Is() const noexcept {
    return hashcode == TypeID<Cmpt>;
  }

  constexpr bool operator<(const CmptType& rhs) const noexcept {
    return hashcode < rhs.hashcode;
  }

  constexpr bool operator==(const CmptType& rhs) const noexcept {
    return hashcode == rhs.hashcode;
  }

  constexpr bool operator!=(const CmptType& rhs) const noexcept {
    return hashcode != rhs.hashcode;
  }

 private:
  size_t hashcode;
  AccessMode mode;
};
}  // namespace My::MyECS

#include "detail/CmptType.inl"
