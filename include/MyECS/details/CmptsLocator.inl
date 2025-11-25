#pragma once

#include <MyTemplate/Func.hpp>

namespace My::MyECS::details {
template <typename... Cmpts>
CmptLocator GenerateCmptLocator(TypeList<Cmpts...>) {
  if constexpr (sizeof...(Cmpts) > 0) {
    constexpr std::array types{AccessTypeID_of<Cmpts>...};
    return CmptLocator{types};
  } else
    return {};
}
}  // namespace My::MyECS::details

namespace My::MyECS {
template <typename Func>
CmptLocator CmptLocator::Generate() {
  using ArgList = FuncTraits_ArgList<std::decay_t<Func>>;
  using CmptList = Filter_t<ArgList, IsNonSingleton>;
  return details::GenerateCmptLocator(CmptList{});
}

template <typename Func>
CmptLocator& CmptLocator::Combine() {
  CmptLocator funcLocator = Generate<Func>();
  for (const auto& type : funcLocator.cmptTypes) cmptTypes.insert(type);
  UpdateGetValue();
  return *this;
}
}  // namespace My::MyECS
