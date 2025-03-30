//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptTag.h"
#include "CmptType.h"

#include <MyContainer/Span.h>

namespace My::MyECS {
// locate components in function's argument list for Archetype
// immutable
class CmptLocator {
 public:
  CmptLocator(Span<const CmptAccessType> types);

  CmptLocator();

  template <typename Func>
  static CmptLocator Generate();

  template <typename Func>
  CmptLocator& Combine();

  size_t HashCode() const noexcept { return hashCode; }

  const CmptAccessTypeSet& CmptAccessTypes() const noexcept {
    return cmptTypes;
  }

  bool operator==(const CmptLocator& rhs) const noexcept;

  bool HasWriteCmptType() const noexcept;

 private:
  void UpdateHashCode() noexcept;

  CmptAccessTypeSet cmptTypes;

  size_t hashCode;
};
}  // namespace My::MyECS

#include "detail/CmptsLocator.inl"
