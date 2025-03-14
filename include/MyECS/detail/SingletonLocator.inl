//
// Created by Admin on 14/03/2025.
//

#pragma once

#include <MyContainer/Algorithm.h>
#include <MyTemplate/Func.h>

namespace My::MyECS::detail {
template <typename... Singletons>
SingletonLocator GenerateSingletonLocator(TypeList<Singletons...>) {
  if constexpr (sizeof...(Singletons) > 0) {
    constexpr std::array<CmptType, sizeof...(Singletons)> types{
        CmptType::Of<Singletons>...};
    return SingletonLocator{types.data(), types.size()};
  } else
    return SingletonLocator{};
}
}  // namespace My::MyECS::detail

namespace My::MyECS {
template <typename Func>
SingletonLocator SingletonLocator::Generate() {
  using ArgList = FuncTraits_ArgList<std::decay_t<Func>>;
  using SingletonList = Filter_t<ArgList, IsSingleton>;
  return detail::GenerateSingletonLocator(SingletonList{});
}

template <typename Func>
SingletonLocator& SingletonLocator::Combine() {
  SingletonLocator funcLocator = Generate<Func>();
  lastFrameSingletonTypes =
      SetUnion(lastFrameSingletonTypes, funcLocator.lastFrameSingletonTypes);
  writeSingletonTypes =
      SetUnion(writeSingletonTypes, funcLocator.writeSingletonTypes);
  latestSingletonTypes =
      SetUnion(latestSingletonTypes, funcLocator.latestSingletonTypes);
  singletonTypes = SetUnion(singletonTypes, funcLocator.singletonTypes);
  return *this;
}
}  // namespace My::MyECS
