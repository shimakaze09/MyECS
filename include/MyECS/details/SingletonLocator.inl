//
// Created by Admin on 14/03/2025.
//

#pragma once

#include <MyTemplate/Func.h>

namespace My::MyECS::details {
template <typename... Singletons>
SingletonLocator GenerateSingletonLocator(TypeList<Singletons...>) {
  if constexpr (sizeof...(Singletons) > 0) {
    constexpr std::array types{AccessTypeID_of<Singletons>...};
    return SingletonLocator{types};
  } else
    return {};
}
}  // namespace My::MyECS::detail

namespace My::MyECS {
template <typename Func>
SingletonLocator SingletonLocator::Generate() {
  using ArgList = FuncTraits_ArgList<std::decay_t<Func>>;
  using SingletonList = Filter_t<ArgList, IsSingleton>;
  return details::GenerateSingletonLocator(SingletonList{});
}

template <typename Func>
SingletonLocator& SingletonLocator::Combine() {
  SingletonLocator funcLocator = Generate<Func>();
  for (const auto& type : funcLocator.singletonTypes)
    singletonTypes.insert(type);
  return *this;
}
}  // namespace My::MyECS
