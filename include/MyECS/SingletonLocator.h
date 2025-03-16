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
  SingletonLocator(const CmptAccessType* types, size_t num);
  SingletonLocator() = default;

  template <typename Func>
  static SingletonLocator Generate();

  template <typename Func>
  SingletonLocator& Combine();

  const std::set<CmptAccessType>& SingletonTypes() const noexcept {
    return singletonTypes;
  }

  bool HasWriteSingletonType() const noexcept;

 private:
  std::set<CmptAccessType> singletonTypes;
};
}  // namespace My::MyECS

#include "detail/SingletonLocator.inl"
