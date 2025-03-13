//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "Util.h"

namespace My::MyECS {
template <typename... AllCmpts, typename... AnyCmpts, typename... NoneCmpts>
EntityFilter::EntityFilter(TypeList<AllCmpts...>, TypeList<AnyCmpts...>,
                           TypeList<NoneCmpts...>)
    : allCmptTypes{CmptType::Of<AllCmpts>...},
      anyCmptTypes{CmptType::Of<AnyCmpts>...},
      noneCmptTypes{CmptType::Of<NoneCmpts>...},
      allHashCode{GenAllHashCode()},
      anyHashCode{GenAnyHashCode()},
      noneHashCode{GenNoneHashCode()},
      combinedHashCode{GenCombinedHashCode()} {
  static_assert(sizeof...(AnyCmpts) != 1);
  static_assert(IsSet_v<TypeList<AllCmpts..., AnyCmpts..., NoneCmpts...>>);
}

template <typename CmptTypeContainer>
void EntityFilter::InsertAll(const CmptTypeContainer& c) {
  if (c.empty()) return;
  for (const auto& type : c) allCmptTypes.insert(type);
  allHashCode = GenAllHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename CmptTypeContainer>
void EntityFilter::InsertAny(const CmptTypeContainer& c) {
  if (c.empty()) return;
  for (const auto& type : c) anyCmptTypes.insert(type);
  anyHashCode = GenAnyHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename CmptTypeContainer>
void EntityFilter::InsertNone(const CmptTypeContainer& c) {
  if (c.empty()) return;
  for (const auto& type : c) noneCmptTypes.insert(type);
  noneHashCode = GenNoneHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename CmptTypeContainer>
void EntityFilter::EraseAll(const CmptTypeContainer& c) {
  if (c.empty()) return;
  for (const auto& type : c) allCmptTypes.erase(type);
  allHashCode = GenAllHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename CmptTypeContainer>
void EntityFilter::EraseAny(const CmptTypeContainer& c) {
  if (c.empty()) return;
  for (const auto& type : c) anyCmptTypes.erase(type);
  anyHashCode = GenAnyHashCode();
  combinedHashCode = GenCombinedHashCode();
}

template <typename CmptTypeContainer>
void EntityFilter::EraseNone(const CmptTypeContainer& c) {
  if (c.empty()) return;
  for (const auto& type : c) noneCmptTypes.erase(type);
  noneHashCode = GenNoneHashCode();
  combinedHashCode = GenCombinedHashCode();
}
}  // namespace My::MyECS

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::MyECS::EntityFilter> {
  size_t operator()(const My::MyECS::EntityFilter& filter) const noexcept {
    return filter.HashCode();
  }
};
}  // namespace std
