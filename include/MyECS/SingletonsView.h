//
// Created by Admin on 14/03/2025.
//

#pragma once

#include "CmptPtr.h"

namespace My::MyECS {
class SingletonsView {
 public:
  SingletonsView(const CmptPtr* singletons, size_t num)
      : singletons{singletons}, num{num} {}

  CmptPtr GetSingleton(CmptType) const;

  const CmptPtr* Singletons() const noexcept { return singletons; }

  size_t NumberOfSingletons() const noexcept { return num; }

 private:
  const CmptPtr* singletons;
  size_t num;
};
}  // namespace My::MyECS
