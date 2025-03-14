//
// Created by Admin on 14/03/2025.
//

#pragma once

#include "CmptTag.h"

#include "CmptType.h"

#include <set>

namespace My::MyECS {
class SingletonLocator {
 public:
  SingletonLocator(const CmptType* types, size_t num);

  SingletonLocator();

  template <typename Func>
  static SingletonLocator Generate();

  template <typename Func>
  SingletonLocator& Combine();

  const std::set<CmptType>& SingletonTypes() const noexcept {
    return singletonTypes;
  }

 private:
  std::set<CmptType> singletonTypes;
};
}  // namespace My::MyECS

#include "detail/SingletonLocator.inl"
