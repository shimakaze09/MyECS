//
// Created by Admin on 14/03/2025.
//

#include <MyECS/SingletonLocator.h>

using namespace My::MyECS;
using namespace std;

SingletonLocator::SingletonLocator(const CmptAccessType* types, size_t num) {
  assert(types || num == 0);
  for (size_t i = 0; i < num; i++) {
    assert(AccessMode_IsSingleton(types[i].GetAccessMode()));
    singletonTypes.insert(types[i]);
  }
}

bool SingletonLocator::HasWriteSingletonType() const noexcept {
  for (const auto& type : singletonTypes) {
    if (type.GetAccessMode() == AccessMode::WRITE_SINGLETON)
      return true;
  }
  return false;
}
