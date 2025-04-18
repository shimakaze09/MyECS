#pragma once

#include <set>
#include <span>

#include "AccessTypeID.hpp"

namespace Smkz::MyECS {
class SingletonLocator {
 public:
  SingletonLocator(std::set<AccessTypeID> types)
      : singletonTypes{std::move(types)} {}
  SingletonLocator(std::span<const AccessTypeID> types);
  SingletonLocator() = default;

  template <typename Func>
  static SingletonLocator Generate();

  template <typename Func>
  SingletonLocator& Combine();

  const std::set<AccessTypeID>& SingletonTypes() const noexcept {
    return singletonTypes;
  }

  bool HasWriteSingletonType() const noexcept;

 private:
  std::set<AccessTypeID> singletonTypes;
};
}  // namespace Smkz::MyECS

#include "details/SingletonLocator.inl"
