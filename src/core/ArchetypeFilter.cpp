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

bool ArchetypeFilter::HaveWriteCmptType() const noexcept {
  for (const auto& type : all) {
    if (type.GetAccessMode() == AccessMode::WRITE)
      return true;
  }
  for (const auto& type : any) {
    if (type.GetAccessMode() == AccessMode::WRITE)
      return true;
  }
  return false;
}

bool ArchetypeFilter::operator==(const ArchetypeFilter& rhs) const noexcept {
  return all == rhs.all && any == rhs.any && none == rhs.none;
}
