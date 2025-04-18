#pragma once

#include <span>

#include "CmptPtr.hpp"

namespace Smkz::MyECS {
class CmptsView {
 public:
  CmptsView() noexcept = default;
  CmptsView(std::span<const CmptAccessPtr> cmpts) noexcept : cmpts{cmpts} {}

  CmptAccessPtr GetCmpt(AccessTypeID) const noexcept;
  std::span<const CmptAccessPtr> Components() const noexcept { return cmpts; }

 private:
  std::span<const CmptAccessPtr> cmpts;
};
}  // namespace Smkz::MyECS
