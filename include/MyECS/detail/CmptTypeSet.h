//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "../EntityQuery.h"
#include "Util.h"

#include <MyTemplate/TemplateList.h>
#include <MyTemplate/TypeID.h>

#include <set>

namespace My::MyECS {
struct CmptTypeSet {
  std::set<CmptType> data;

  size_t HashCode() const;

  void Insert(const CmptType* types, size_t num);
  void Erase(const CmptType* types, size_t num);
  bool Contains(CmptType type) const;

  template <typename CmptTypeContainer>
  bool Contains(const CmptTypeContainer& types) const;

  template <typename CmptTypeContainer>
  bool ContainsAny(const CmptTypeContainer& types) const;

  template <typename CmptTypeContainer>
  bool NotContain(const CmptTypeContainer& types) const;

  bool IsMatch(const ArchetypeFilter& filter) const;

  bool IsMatch(const CmptLocator& locator) const;

  bool IsMatch(const EntityQuery& query) const;

  bool operator==(const CmptTypeSet& rhs) const { return data == rhs.data; }
};
}  // namespace My::MyECS

#include "CmptTypeSet.inl"
