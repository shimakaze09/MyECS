#pragma once

#include <MyTemplate/Func.hpp>

namespace Smkz::MyECS::details {
template <typename... Singletons>
SingletonLocator GenerateSingletonLocator(TypeList<Singletons...>) {
  if constexpr (sizeof...(Singletons) > 0) {
    constexpr std::array types{AccessTypeID_of<Singletons>...};
    return SingletonLocator{types};
  } else
    return {};
}
}  // namespace Smkz::MyECS::details

namespace Smkz::MyECS {
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
}  // namespace Smkz::MyECS
