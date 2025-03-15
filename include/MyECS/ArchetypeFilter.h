//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptType.h"

#include <MyTemplate/TypeList.h>

#include <set>

namespace My::MyECS {
// filter Archetype with All, Any and None
struct ArchetypeFilter {
  std::set<CmptType> all;
  std::set<CmptType> any;
  std::set<CmptType> none;

  size_t HashCode() const noexcept;

  bool operator==(const ArchetypeFilter& rhs) const;
};
}  // namespace My::MyECS
