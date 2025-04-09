//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "AccessTypeID.h"
#include "CmptTag.h"

#include <span>

namespace My::MyECS {
// locate components in function's argument list for Archetype
// immutable
class CmptLocator {
public:
  CmptLocator(std::span<const AccessTypeID> types);
  CmptLocator(AccessTypeIDSet types);

  CmptLocator();

  template<typename Func>
  static CmptLocator Generate();

  template<typename Func>
  CmptLocator& Combine();

  std::size_t GetValue() const noexcept { return hashCode; }

  const AccessTypeIDSet& AccessTypeIDs() const noexcept { return cmptTypes; }

  bool operator==(const CmptLocator& rhs) const noexcept;

  bool HasWriteTypeID() const noexcept;
private:
  void UpdateGetValue() noexcept;

  AccessTypeIDSet cmptTypes;

  std::size_t hashCode;
};
}  // namespace My::MyECS

#include "detail/CmptsLocator.inl"
