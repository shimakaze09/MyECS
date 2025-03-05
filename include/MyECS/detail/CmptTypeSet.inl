//
// Created by Admin on 5/03/2025.
//

#pragma once

namespace My {
template <typename... Cmpts>
CmptTypeSet::CmptTypeSet(TypeList<Cmpts...>)
    : std::set<CmptType>{CmptType::Of<Cmpts>()...},
      hashcode{HashCodeOf<Cmpts...>()} {}

template <typename... Cmpts>
constexpr size_t CmptTypeSet::HashCodeOf() noexcept {
  return HashCodeOf(QuickSort_t<TypeList<Cmpts...>, TypeID_Less>{});
}

template <typename... Cmpts>
void CmptTypeSet::Insert() {
  (insert(CmptType::Of<Cmpts>()), ...);
  hashcode = HashCodeOf(*this);
}

template <typename... Cmpts>
void CmptTypeSet::Erase() noexcept {
  (erase(CmptType::Of<Cmpts>()), ...);
  hashcode = HashCodeOf(*this);
}

template <typename... Cmpts>
constexpr bool CmptTypeSet::IsContain() const {
  if constexpr (sizeof...(Cmpts) == 0)
    return true;
  else
    return ((find(CmptType::Of<Cmpts>()) != cend()) && ...);
}

template <typename... Cmpts>
constexpr bool CmptTypeSet::IsContain(TypeList<Cmpts...>) const {
  return IsContain<Cmpts...>();
}

bool CmptTypeSet::IsContain(CmptType type) const {
  return find(type) != cend();
}

template <typename CmptTypeContainer>
bool CmptTypeSet::IsContain(const CmptTypeContainer& types) const {
  for (auto type : types) {
    if (!IsContain(type))
      return false;
  }
  return true;
}

template <typename... Cmpts>
constexpr bool CmptTypeSet::IsContainAny() const {
  if constexpr (sizeof...(Cmpts) == 0)
    return true;
  else
    return ((find(CmptType::Of<Cmpts>()) != end()) || ...);
}

template <typename... Cmpts>
constexpr bool CmptTypeSet::IsContainAny(TypeList<Cmpts...>) const {
  return IsContainAny<Cmpts...>();
}

template <typename CmptTypeContainer>
bool CmptTypeSet::IsContainAny(const CmptTypeContainer& types) const {
  if (types.empty())
    return true;

  for (auto type : types) {
    if (IsContain(type))
      return true;
  }

  return false;
}

template <typename... Cmpts>
constexpr bool CmptTypeSet::IsNotContain() const {
  if constexpr (sizeof...(Cmpts) == 0)
    return true;
  else
    return ((find(CmptType::Of<Cmpts>()) == end()) && ...);
}

template <typename... Cmpts>
constexpr bool CmptTypeSet::IsNotContain(TypeList<Cmpts...>) const {
  return IsNotContain<Cmpts...>();
}

bool CmptTypeSet::IsNotContain(CmptType type) const {
  return find(type) == cend();
}

template <typename CmptTypeContainer>
bool CmptTypeSet::IsNotContain(const CmptTypeContainer& types) const {
  for (auto type : types) {
    if (IsContain(type))
      return false;
  }
  return true;
}

bool CmptTypeSet::IsMatch(const EntityFilter& filter) const {
  return IsContain(filter.AllCmptTypes()) &&
         IsContainAny(filter.AnyCmptTypes()) &&
         IsNotContain(filter.NoneCmptTypes());
}

bool CmptTypeSet::IsMatch(const EntityLocator& locator) const {
  return IsContain(locator.CmptTypes());
}

bool CmptTypeSet::IsMatch(const EntityQuery& query) const {
  return IsMatch(query.filter) && IsMatch(query.locator);
}

template <typename... Cmpts>
bool CmptTypeSet::Is() const {
  return sizeof...(Cmpts) == size() && IsContain<Cmpts...>();
}

template <typename... Cmpts>
static constexpr size_t CmptTypeSet::HashCodeOf(TypeList<Cmpts...>) noexcept {
  return HashCodeOf(
      std::array<CmptType, sizeof...(Cmpts)>{CmptType::Of<Cmpts>()...});
}

template <typename Container>
static constexpr size_t CmptTypeSet::HashCodeOf(const Container& cmpts) {
  size_t seed = TypeID<CmptTypeSet>;
  for (const CmptType& cmpt : cmpts)
    seed = hash_combine(seed, cmpt.HashCode());
  return seed;
}
}  // namespace My

namespace std {
template <>
struct hash<My::CmptTypeSet> {
  size_t operator()(const My::CmptTypeSet& types) const noexcept {
    return types.HashCode();
  }
};
}  // namespace std
