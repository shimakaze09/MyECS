//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptType.h"

#include <MyTemplate/TypeList.h>

#include <set>

namespace My {
// filter Archetype with All, Any and None
class EntityFilter {
 public:
  EntityFilter();

  template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts>
  EntityFilter(TypeList<AllCmpts...> allList, TypeList<AnyCmpts...> anyList,
               TypeList<NoneCmpts...> noneList);

  template <typename... AllCmpts>
  static EntityFilter CreateAll() {
    return {TypeList<AllCmpts...>, TypeList<>{}, TypeList<>{}};
  }

  template <typename... AnyCmpts>
  static EntityFilter CreateAny() {
    return {TypeList<>{}, TypeList<AnyCmpts...>, TypeList<>{}};
  }

  template <typename... NoneCmpts>
  static EntityFilter CreateNone() {
    return {TypeList<>{}, TypeList<>{}, TypeList<NoneCmpts...>};
  }

  EntityFilter(std::set<CmptType> allCmptTypes,
               std::set<CmptType> anyCmptTypes = {},
               std::set<CmptType> noneCmptTypes = {});

  size_t HashCode() const noexcept { return combinedHashCode; }

  const std::set<CmptType>& AllCmptTypes() const noexcept {
    return allCmptTypes;
  }

  const std::set<CmptType>& AnyCmptTypes() const noexcept {
    return anyCmptTypes;
  }

  const std::set<CmptType>& NoneCmptTypes() const noexcept {
    return noneCmptTypes;
  }

  // [API]
  // <Mode><Type>(Container|CmptTypes...|CmptType*, num)
  // - <Mode>: Insert | Erase
  // - <Type>: All | Any | None
  // - side effect: update hashcode

  void InsertAll(const CmptType* types, size_t num);
  void InsertAny(const CmptType* types, size_t num);
  void InsertNone(const CmptType* types, size_t num);
  void EraseAll(const CmptType* types, size_t num);
  void EraseAny(const CmptType* types, size_t num);
  void EraseNone(const CmptType* types, size_t num);

  template <typename Container>
  void InsertAll(const Container&);
  template <typename Container>
  void InsertAny(const Container&);
  template <typename Container>
  void InsertNone(const Container&);
  template <typename Container>
  void EraseAll(const Container&);
  template <typename Container>
  void EraseAny(const Container&);
  template <typename Container>
  void EraseNone(const Container&);

  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void InsertAll(CmptTypes...);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void InsertAny(CmptTypes...);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void InsertNone(CmptTypes...);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void EraseAll(CmptTypes...);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void EraseAny(CmptTypes...);
  template <
      typename... CmptTypes,
      // for function overload
      typename = std::enable_if_t<(std::is_same_v<CmptTypes, CmptType> && ...)>>
  void EraseNone(CmptTypes...);

  bool operator==(const EntityFilter& filter) const noexcept;

 private:
  size_t GenAllHashCode() const noexcept;
  size_t GenAnyHashCode() const noexcept;
  size_t GenNoneHashCode() const noexcept;
  size_t GenCombinedHashCode() const noexcept;

  std::set<CmptType> allCmptTypes;
  std::set<CmptType> anyCmptTypes;
  std::set<CmptType> noneCmptTypes;

  size_t allHashCode;
  size_t anyHashCode;
  size_t noneHashCode;

  size_t combinedHashCode;
};
}  // namespace My

#include "detail/EntityFilter.inl"
