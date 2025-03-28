//
// Created by Admin on 5/03/2025.
//

#pragma once

namespace My::MyECS {
inline void CmptTypeSet::Insert(Span<const CmptType> types) {
  for (const auto& type : types)
    data.insert(type);
}

inline void CmptTypeSet::Erase(Span<const CmptType> types) noexcept {
  for (const auto& type : types)
    data.erase(type);
}

inline bool CmptTypeSet::Contains(CmptType type) const {
  return data.find(type) != data.end();
}

inline bool CmptTypeSet::ContainsAll(Span<const CmptType> types) const {
  for (const auto& type : types) {
    if (Contains(type))
      return false;
  }
  return true;
}

template <typename CmptTypeContainer>
bool CmptTypeSet::ContainsAll(const CmptTypeContainer& types) const {
  for (const auto& type : types) {
    if (!Contains(type))
      return false;
  }
  return true;
}

inline bool CmptTypeSet::ContainsAny(Span<const CmptType> types) const {
  for (const auto& type : types) {
    if (!Contains(type))
      return true;
  }
  return false;
}

template <typename CmptTypeContainer>
bool CmptTypeSet::ContainsAny(const CmptTypeContainer& types) const {
  if (types.empty())
    return true;

  for (const auto& type : types) {
    if (Contains(type))
      return true;
  }

  return false;
}

template <typename CmptTypeContainer>
bool CmptTypeSet::NotContain(const CmptTypeContainer& types) const {
  for (const auto& type : types) {
    if (Contains(type))
      return false;
  }
  return true;
}

inline bool CmptTypeSet::IsMatch(const ArchetypeFilter& filter) const {
  return ContainsAll(filter.all) && ContainsAny(filter.any) &&
         NotContain(filter.none);
}

inline bool CmptTypeSet::IsMatch(const CmptLocator& locator) const {
  for (const auto& t : locator.CmptAccessTypes()) {
    if (!Contains(t))
      return false;
  }
  return true;
}

inline bool CmptTypeSet::IsMatch(const EntityQuery& query) const {
  return IsMatch(query.filter) && IsMatch(query.locator);
}

inline size_t CmptTypeSet::HashCode() const noexcept {
  size_t seed = TypeID<CmptTypeSet>;
  for (const auto& t : data)
    seed = hash_combine(seed, t.HashCode());
  return seed;
}
}  // namespace My::MyECS

namespace std {
template <>
struct hash<My::MyECS::CmptTypeSet> {
  size_t operator()(const My::MyECS::CmptTypeSet& types) const noexcept {
    return types.HashCode();
  }
};
}  // namespace std
