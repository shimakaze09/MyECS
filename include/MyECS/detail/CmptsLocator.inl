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
    constexpr std::array<CmptType, sizeof...(Cmpts)> types{
        CmptType::Of<Cmpts>...};
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
  lastFrameCmptTypes =
      SetUnion(lastFrameCmptTypes, funcLocator.lastFrameCmptTypes);
  writeCmptTypes = SetUnion(writeCmptTypes, funcLocator.writeCmptTypes);
  latestCmptTypes = SetUnion(latestCmptTypes, funcLocator.latestCmptTypes);
  cmptTypes = SetUnion(cmptTypes, funcLocator.cmptTypes);
  return *this;
}
}  // namespace My::MyECS
