//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <set>

#include "CmptTag.h"
#include "CmptType.h"

namespace My::MyECS {
// locate components in function's argument list for Archetype
// TODO: combine with a system function's locator
class CmptLocator {
 public:
  CmptLocator(const CmptType* types, size_t num);

  CmptLocator();

  template <typename Func>
  static CmptLocator Generate();

  template <typename Func>
  CmptLocator& Combine();

  size_t HashCode() const noexcept { return hashCode; }

  const std::set<CmptType>& LastFrameCmptTypes() const noexcept {
    return lastFrameCmptTypes;
  }

  const std::set<CmptType>& WriteCmptTypes() const noexcept {
    return writeCmptTypes;
  }

  const std::set<CmptType>& LatestCmptTypes() const noexcept {
    return latestCmptTypes;
  }

  const std::set<CmptType>& CmptTypes() const noexcept { return cmptTypes; }

  bool operator==(const CmptLocator& rhs) const noexcept;

 private:
  size_t GenHashCode() const noexcept;

  std::set<CmptType> lastFrameCmptTypes;
  std::set<CmptType> writeCmptTypes;
  std::set<CmptType> latestCmptTypes;
  std::set<CmptType> cmptTypes;

  size_t hashCode;
};
}  // namespace My::MyECS

#include "detail/CmptsLocator.inl"
