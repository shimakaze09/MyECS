//
// Created by Admin on 13/03/2025.
//

#pragma once

#include <MyContainer/Span.h>
#include "CmptPtr.h"

namespace My::MyECS {
class CmptsView {
 public:
  CmptsView() noexcept = default;

  CmptsView(Span<const CmptAccessPtr> cmpts) noexcept : cmpts{cmpts} {}

  CmptAccessPtr GetCmpt(CmptAccessType) const noexcept;

  Span<const CmptAccessPtr> Components() const noexcept { return cmpts; }

 private:
  Span<const CmptAccessPtr> cmpts;
};
}  // namespace My::MyECS
