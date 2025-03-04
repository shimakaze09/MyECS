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
      combinedHashCode{hash_combine(
          std::array<size_t, 3>{allHashCode, anyHashCode, noneHashCode})} {
  static_assert(sizeof...(AnyCmpts) != 1);
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
