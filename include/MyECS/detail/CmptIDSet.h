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
  bool IsContain() const noexcept {
    return ((find(TypeID<Cmpts>) != end()) && ...);
  }

  bool IsContain(const CmptIDSet& ids) const noexcept {
    for (auto id : ids) {
      if (!IsContain(id))
        return false;
    }
    return true;
  }

  bool IsContain(size_t cmptHash) const noexcept {
    return find(cmptHash) != end();
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
