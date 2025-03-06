//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "Util.h"

namespace My {
template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts>
EntityFilter::EntityFilter(TypeList<AllCmpts...>, TypeList<AnyCmpts...>,
                           TypeList<NoneCmpts...>)
    : allCmptTypes{CmptType::Of<AllCmpts>()...},
      anyCmptTypes{CmptType::Of<AnyCmpts>()...},
      noneCmptTypes{CmptType::Of<NoneCmpts>()...},
      allHashCode{GenAllHashCode()},
      anyHashCode{GenAnyHashCode()},
      noneHashCode{GenNoneHashCode()},
      combinedHashCode{GenCombinedHashCode()} {
  static_assert(sizeof...(AnyCmpts) != 1);
  static_assert(IsSet_v<TypeList<AllCmpts..., AnyCmpts..., NoneCmpts...>>);
}

template <typename Container>
void EntityFilter::InsertAll(const Container& c) {
  if (c.empty())
    return;
  for (const auto& type : c)
    allCmptTypes.insert(type);
  allHashCode = GenAllHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename Container>
void EntityFilter::InsertAny(const Container& c) {
  if (c.empty())
    return;
  for (const auto& type : c)
    anyCmptTypes.insert(type);
  anyHashCode = GenAnyHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename Container>
void EntityFilter::InsertNone(const Container& c) {
  if (c.empty())
    return;
  for (const auto& type : c)
    noneCmptTypes.insert(type);
  noneHashCode = GenNoneHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename Container>
void EntityFilter::EraseAll(const Container& c) {
  if (c.empty())
    return;
  for (const auto& type : c)
    allCmptTypes.erase(type);
  allHashCode = GenAllHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename Container>
void EntityFilter::EraseAny(const Container& c) {
  if (c.empty())
    return;
  for (const auto& type : c)
    anyCmptTypes.erase(type);
  anyHashCode = GenAnyHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename Container>
void EntityFilter::EraseNone(const Container& c) {
  if (c.empty())
    return;
  for (const auto& type : c)
    noneCmptTypes.erase(type);
  noneHashCode = GenNoneHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename... CmptTypes, typename>
void EntityFilter::InsertAll(CmptTypes... types) {
  static_assert(sizeof...(CmptTypes) > 0);
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr;
  InsertAll(typeArr.data(), typeArr.size());
}

template <typename... CmptTypes, typename>
void EntityFilter::InsertAny(CmptTypes... types) {
  static_assert(sizeof...(CmptTypes) > 0);
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr;
  InsertAny(typeArr.data(), typeArr.size());
}

template <typename... CmptTypes, typename>
void EntityFilter::InsertNone(CmptTypes... types) {
  static_assert(sizeof...(CmptTypes) > 0);
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr;
  InsertNone(typeArr.data(), typeArr.size());
}

template <typename... CmptTypes, typename>
void EntityFilter::EraseAll(CmptTypes... types) {
  static_assert(sizeof...(CmptTypes) > 0);
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr;
  EraseAll(typeArr.data(), typeArr.size());
}

template <typename... CmptTypes, typename>
void EntityFilter::EraseAny(CmptTypes... types) {
  static_assert(sizeof...(CmptTypes) > 0);
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr;
  EraseAny(typeArr.data(), typeArr.size());
}

template <typename... CmptTypes, typename>
void EntityFilter::EraseNone(CmptTypes... types) {
  static_assert(sizeof...(CmptTypes) > 0);
  const std::array<CmptType, sizeof...(CmptTypes)> typeArr;
  EraseNone(typeArr.data(), typeArr.size());
}
}  // namespace My

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::EntityFilter> {
  size_t operator()(const My::EntityFilter& filter) const noexcept {
    return filter.HashCode();
  }
};
}  // namespace std
