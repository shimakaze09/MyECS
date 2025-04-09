//
// Created by Admin on 14/03/2025.
//

#pragma once

#include "CmptPtr.h"

#include <span>

namespace My::MyECS {
class SingletonsView {
 public:
  SingletonsView(std::span<const CmptAccessPtr> singletons) noexcept
      : singletons{singletons} {}

  CmptAccessPtr GetSingleton(AccessTypeID) const noexcept;

  std::span<const CmptAccessPtr> Singletons() const noexcept {
    return singletons;
  }

 private:
  std::span<const CmptAccessPtr> singletons;
};
}  // namespace My::MyECS
