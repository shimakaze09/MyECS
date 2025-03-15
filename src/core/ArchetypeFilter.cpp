//
// Created by Admin on 4/03/2025.
//

#include <MyECS/ArchetypeFilter.h>

#include <MyECS/detail/Util.h>

using namespace My::MyECS;

size_t ArchetypeFilter::HashCode() const noexcept {
  size_t rst = TypeID<ArchetypeFilter>;
  for (const auto& type : all)
    rst = hash_combine(rst, type.HashCode());
  for (const auto& type : any)
    rst = hash_combine(rst, type.HashCode());
  for (const auto& type : none)
    rst = hash_combine(rst, type.HashCode());
  return rst;
}

bool ArchetypeFilter::operator==(const ArchetypeFilter& rhs) const {
  return all == rhs.all && any == rhs.any && none == rhs.none;
}
