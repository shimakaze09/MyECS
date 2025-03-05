//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptTag.h"

#include "CmptType.h"

#include <set>

namespace My {
class EntityLocator {
 public:
  template <typename TaggedCmptList>
  EntityLocator(TaggedCmptList);

  template <typename... LastFrameCmpts, typename... WriteCmpts,
            typename... LatestCmpts>
  EntityLocator(TypeList<LastFrameCmpts...>, TypeList<WriteCmpts...>,
                TypeList<LatestCmpts...>);

  EntityLocator(std::set<CmptType> lastFrameCmpts = {},
                std::set<CmptType> writeFrameCmpts = {},
                std::set<CmptType> latestCmpts = {});

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

  static const EntityLocator& InvalidInstance() noexcept {
    static EntityLocator invalid_instance{TypeList{}, TypeList{}, TypeList{}};
    return invalid_instance;
  }

  bool operator==(const EntityLocator& locator) const noexcept;

 private:
  size_t GenHashCode() const noexcept;

  std::set<CmptType> lastFrameCmptTypes;
  std::set<CmptType> writeCmptTypes;
  std::set<CmptType> latestCmptTypes;
  std::set<CmptType> cmptTypes;

  size_t hashCode;
};
}  // namespace My

#include "detail/EntityLocator.inl"
