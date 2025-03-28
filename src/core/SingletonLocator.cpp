//
// Created by Admin on 14/03/2025.
//

#include <MyECS/SingletonLocator.h>

using namespace My::MyECS;
using namespace std;

SingletonLocator::SingletonLocator(Span<const CmptAccessType> types) {
  for (const auto& type : types)
    singletonTypes.insert(type);
}

bool SingletonLocator::HasWriteSingletonType() const noexcept {
  for (const auto& type : singletonTypes) {
    if (type.GetAccessMode() == AccessMode::WRITE)
      return true;
  }
  return false;
}
