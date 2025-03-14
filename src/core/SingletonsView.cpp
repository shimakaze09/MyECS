//
// Created by Admin on 14/03/2025.
//

#include <MyECS/SingletonsView.h>
 
 using namespace My::MyECS;
 
CmptPtr SingletonsView::GetSingleton(CmptType t) const {
  for (size_t i = 0; i < num; i++) {
    if (singletons[i].Type() == t) {
      assert(singletons[i].Type().GetAccessMode() == t.GetAccessMode());
      return singletons[i];
    }
  }
  return { CmptType::Invalid(), nullptr };
}
