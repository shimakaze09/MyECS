//
// Created by Admin on 14/03/2025.
//

#pragma once

#include <MyContainer/Algorithm.h>
#include <MyTemplate/Func.h>

namespace My::MyECS::detail {
template <typename... Cmpts>
CmptLocator GenerateCmptLocator(TypeList<Cmpts...>) {
  if constexpr (sizeof...(Cmpts) > 0) {
    constexpr std::array types{CmptAccessType::Of<Cmpts>...};
    return CmptLocator{types.data(), types.size()};
  } else
    return CmptLocator{};
}
}  // namespace My::MyECS::detail

namespace My::MyECS {
template <typename Func>
CmptLocator CmptLocator::Generate() {
  using ArgList = FuncTraits_ArgList<std::decay_t<Func>>;
  using CmptList = Filter_t<ArgList, IsNonSingleton>;
  return detail::GenerateCmptLocator(CmptList{});
}

template <typename Func>
CmptLocator& CmptLocator::Combine() {
  CmptLocator funcLocator = Generate<Func>();
  cmptTypes = SetUnion(cmptTypes, funcLocator.cmptTypes);
  UpdateHashCode();
  return *this;
}
}  // namespace My::MyECS
