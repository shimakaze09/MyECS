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
  for (size_t i = 0; i < num; i++) {
    switch (types[i].GetAccessMode()) {
      case My::MyECS::AccessMode::LAST_FRAME:
        lastFrameCmptTypes.insert(types[i]);
        break;
      case My::MyECS::AccessMode::WRITE:
        writeCmptTypes.insert(types[i]);
        break;
      case My::MyECS::AccessMode::LATEST:
        latestCmptTypes.insert(types[i]);
        break;
      default:
        assert(false);
        break;
    }
  }
  cmptTypes = SetUnion(lastFrameCmptTypes, writeCmptTypes);
  cmptTypes = SetUnion(cmptTypes, latestCmptTypes);

  hashCode = GenHashCode();
}

CmptLocator::CmptLocator() : hashCode{TypeID<CmptLocator>} {}

size_t CmptLocator::GenHashCode() const noexcept {
  size_t rst = TypeID<CmptLocator>;
  for (auto type : cmptTypes)
    rst = hash_combine(rst, type.HashCode());
  return rst;
}

bool CmptLocator::operator==(const CmptLocator& rhs) const noexcept {
  return lastFrameCmptTypes == rhs.lastFrameCmptTypes &&
         writeCmptTypes == rhs.writeCmptTypes &&
         latestCmptTypes == rhs.latestCmptTypes;
}
