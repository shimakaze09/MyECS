//
// Created by Admin on 14/03/2025.
//

#pragma once

#include "CmptPtr.h"

namespace My::MyECS {
class SingletonsView {
 public:
  SingletonsView(const CmptAccessPtr* singletons, size_t num) noexcept
      : singletons{singletons}, num{num} {}

  CmptAccessPtr GetSingleton(CmptAccessType) const;

  const CmptAccessPtr* Singletons() const noexcept { return singletons; }

  size_t NumberOfSingletons() const noexcept { return num; }

 private:
  const CmptAccessPtr* singletons;
  size_t num;
};
}  // namespace My::MyECS
