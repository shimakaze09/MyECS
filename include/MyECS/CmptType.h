//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptTag.h"

#include "detail/Util.h"

#include <MyTemplate/TypeID.h>

namespace My::MyECS {
// Component Type
// use a hashcode to distinguish different type
class CmptType {
 public:
  explicit constexpr CmptType(size_t id) noexcept : hashcode{id} {}

  explicit constexpr CmptType(std::string_view type_name) noexcept
      : hashcode{RuntimeTypeID(type_name)} {}

  template <typename Cmpt, std::enable_if_t<!IsTaggedCmpt_v<Cmpt>, int> = 0>
  static constexpr CmptType Of = CmptType{TypeID<Cmpt>};

  constexpr size_t HashCode() const noexcept { return hashcode; }

  static constexpr CmptType Invalid() noexcept {
    return CmptType{size_t_invalid};
  }

  constexpr bool Valid() const noexcept { return hashcode == size_t_invalid; }

  template <typename Cmpt>
  constexpr bool Is() const noexcept;

  constexpr bool operator<(const CmptType& rhs) const noexcept {
    return hashcode < rhs.hashcode;
  }

  constexpr bool operator<=(const CmptType& rhs) const noexcept {
    return hashcode <= rhs.hashcode;
  }

  constexpr bool operator>(const CmptType& rhs) const noexcept {
    return hashcode > rhs.hashcode;
  }

  constexpr bool operator>=(const CmptType& rhs) const noexcept {
    return hashcode >= rhs.hashcode;
  }

  constexpr bool operator==(const CmptType& rhs) const noexcept {
    return hashcode == rhs.hashcode;
  }

  constexpr bool operator!=(const CmptType& rhs) const noexcept {
    return hashcode != rhs.hashcode;
  }

 private:
  size_t hashcode;
};

// CmptType with AccessMode
class CmptAccessType {
 public:
  constexpr CmptAccessType(size_t id, AccessMode mode) noexcept
      : type{id}, mode{mode} {}

  constexpr CmptAccessType(std::string_view type_name, AccessMode mode) noexcept
      : type{RuntimeTypeID(type_name)}, mode{mode} {}

  constexpr CmptAccessType(CmptType type, AccessMode mode) noexcept
      : type{type}, mode{mode} {}

  explicit constexpr CmptAccessType(CmptType type) noexcept
      : CmptAccessType{type, AccessMode::LATEST} {}

  template <typename Cmpt, AccessMode mode = AccessMode::LATEST>
  static constexpr CmptAccessType Of = CmptAccessType{
      CmptType::Of<RemoveTag_t<Cmpt>>, AccessModeOf_default<Cmpt, mode>};

  // same with CmptType's HashCode
  constexpr size_t HashCode() const noexcept { return type.HashCode(); }

  constexpr CmptType GetCmptType() const noexcept { return type; }

  constexpr AccessMode GetAccessMode() const noexcept { return mode; }

  constexpr operator CmptType() const noexcept { return type; }

  static constexpr CmptAccessType Invalid() noexcept {
    return CmptAccessType{size_t_invalid, AccessMode::LATEST};
  }

  constexpr bool Valid() const noexcept { return type.Valid(); }

  // same with CmptType's operator<
  constexpr bool operator<(const CmptAccessType& rhs) const noexcept {
    return type < rhs.type;
  }

  constexpr bool operator<=(const CmptAccessType& rhs) const noexcept {
    return type <= rhs.type;
  }

  constexpr bool operator>(const CmptAccessType& rhs) const noexcept {
    return type > rhs.type;
  }

  constexpr bool operator>=(const CmptAccessType& rhs) const noexcept {
    return type >= rhs.type;
  }

  constexpr bool operator==(const CmptAccessType& rhs) const noexcept {
    return type == rhs.type;
  }

  constexpr bool operator!=(const CmptAccessType& rhs) const noexcept {
    return type != rhs.type;
  }

 private:
  CmptType type;
  AccessMode mode;
};
}  // namespace My::MyECS

#include "detail/CmptType.inl"
