//
// Created by Admin on 28/02/2025.
//

#pragma once

#include <MyTemplate/TemplateList.h>
#include <MyTemplate/TypeID.h>

#include <set>

namespace My {
class CmptIDSet : std::set<size_t> {
 public:
  CmptIDSet() = default;

  template <typename... Cmpts>
  CmptIDSet(TypeList<Cmpts...>) noexcept : std::set<size_t>{TypeID<Cmpts>...} {}

  template <typename... Cmpts>
  void Add() noexcept {
    (insert(TypeID<Cmpts>), ...);
  }

  template <typename... Cmpts>
  void Remove() noexcept {
    (erase(TypeID<Cmpts>), ...);
  }

  template <typename... Cmpts>
  constexpr bool IsContain() const noexcept {
    if constexpr (sizeof...(Cmpts) == 0)
      return true;
    else
      return ((find(TypeID<Cmpts>) != end()) && ...);
  }

  template <typename... Cmpts>
  constexpr bool IsContain(TypeList<Cmpts...>) const noexcept {
    return IsContain<Cmpts...>();
  }

  bool IsContain(size_t cmptHash) const noexcept {
    return find(cmptHash) != end();
  }

  template <typename IDContainer>
  bool IsContain(const IDContainer& ids) const noexcept {
    for (auto id : ids) {
      if (!IsContain(id))
        return false;
    }
    return true;
  }

  template <typename... Cmpts>
  constexpr bool IsContainAny() const noexcept {
    if constexpr (sizeof...(Cmpts) == 0)
      return true;
    else
      return ((find(TypeID<Cmpts>) != end()) || ...);
  }

  template <typename... Cmpts>
  constexpr bool IsContainAny(TypeList<Cmpts...>) const noexcept {
    return IsContainAny<Cmpts...>();
  }

  template <typename IDContainer>
  bool IsContainAny(const IDContainer& ids) const noexcept {
    if (ids.empty())
      return true;

    for (auto id : ids) {
      if (IsContain(id))
        return true;
    }

    return false;
  }

  template <typename... Cmpts>
  constexpr bool IsNotContain() const noexcept {
    if constexpr (sizeof...(Cmpts) == 0)
      return true;
    else
      return ((find(TypeID<Cmpts>) == end()) && ...);
  }

  template <typename... Cmpts>
  constexpr bool IsNotContain(TypeList<Cmpts...>) const noexcept {
    return IsNotContain<Cmpts...>();
  }

  bool IsNotContain(size_t cmptHash) const noexcept {
    return find(cmptHash) == end();
  }

  template <typename IDContainer>
  bool IsNotContain(const IDContainer& ids) const noexcept {
    for (auto id : ids) {
      if (IsContain(id))
        return false;
    }
    return true;
  }

  template <typename... Cmpts>
  bool Is() const noexcept {
    return sizeof...(Cmpts) == size() && IsContain<Cmpts...>();
  }

  using std::set<size_t>::begin;
  using std::set<size_t>::end;
  using std::set<size_t>::size;

  friend bool operator<(const CmptIDSet& x, const CmptIDSet& y) noexcept {
    return static_cast<const std::set<size_t>&>(x) <
           static_cast<const std::set<size_t>&>(y);
  }

  friend bool operator==(const CmptIDSet& x, const CmptIDSet& y) noexcept {
    return static_cast<const std::set<size_t>&>(x) ==
           static_cast<const std::set<size_t>&>(y);
  }
};
}  // namespace My
