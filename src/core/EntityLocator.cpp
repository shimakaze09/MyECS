//
// Created by Admin on 4/03/2025.
//

#include <MyECS/EntityLocator.h>

#include <MyECS/detail/Util.h>

using namespace My;

size_t EntityLocator::GenHashCode() const noexcept {
  size_t rst = TypeID<EntityLocator>;
  for (auto type : cmptTypes)
    rst = hash_combine(rst, type.HashCode());
  return rst;
}

bool EntityLocator::operator==(const EntityLocator& locator) const noexcept {
  return lastFrameCmptTypes == locator.lastFrameCmptTypes &&
         writeCmptTypes == locator.writeCmptTypes &&
         latestCmptTypes == latestCmptTypes;
}
