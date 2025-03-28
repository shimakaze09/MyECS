//
// Created by Admin on 14/03/2025.
//

#pragma once

#include "CmptPtr.h"

#include <MyContainer/Span.h>

namespace My::MyECS {
class SingletonsView {
 public:
  SingletonsView(Span<const CmptAccessPtr> singletons) noexcept
      : singletons{singletons} {}

  CmptAccessPtr GetSingleton(CmptAccessType) const noexcept;

  Span<const CmptAccessPtr> Singletons() const noexcept { return singletons; }

 private:
  Span<const CmptAccessPtr> singletons;
};
}  // namespace My::MyECS
