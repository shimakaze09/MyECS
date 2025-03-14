//
// Created by Admin on 14/03/2025.
//

#include <MyECS/SingletonLocator.h>
 
 #include <MyECS/detail/Util.h>
 
 #include <MyContainer/Algorithm.h>
 
 using namespace My::MyECS;
using namespace std;
 
SingletonLocator::SingletonLocator(const CmptType* types, size_t num) {
  assert(types != nullptr && num > 0);
  for (size_t i = 0; i < num; i++) {
    switch (types[i].GetAccessMode())
    {
      case My::MyECS::AccessMode::LAST_FRAME_SINGLETON:
        lastFrameSingletonTypes.insert(types[i]);
      break;
      case My::MyECS::AccessMode::WRITE_SINGLETON:
        writeSingletonTypes.insert(types[i]);
      break;
      case My::MyECS::AccessMode::LATEST_SINGLETON:
        latestSingletonTypes.insert(types[i]);
      break;
      default:
        assert(false);
      break;
    }
  }
 
  singletonTypes = SetUnion(lastFrameSingletonTypes, writeSingletonTypes);
  singletonTypes = SetUnion(singletonTypes, latestSingletonTypes);
}
 
SingletonLocator::SingletonLocator() {}
