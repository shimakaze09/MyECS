//
// Created by Admin on 14/03/2025.
//

#include <MyECS/CmptLocator.h>

#include <MyECS/detail/Util.h>

#include <MyContainer/Algorithm.h>

using namespace My::MyECS;
using namespace std;

CmptLocator::CmptLocator(const CmptType* types, size_t num) {
  assert(types != nullptr && num > 0);
  for (size_t i = 0; i < num; i++)
    cmptTypes.insert(types[i]);

  hashCode = GenHashCode();
}

CmptLocator::CmptLocator() : hashCode{TypeID<CmptLocator>} {}

size_t CmptLocator::GenHashCode() const noexcept {
  size_t rst = TypeID<CmptLocator>;
  for (const auto& type : cmptTypes)
    rst = hash_combine(rst, type.HashCode());
  return rst;
}

bool CmptLocator::operator==(const CmptLocator& rhs) const {
  return cmptTypes == rhs.cmptTypes;
}
