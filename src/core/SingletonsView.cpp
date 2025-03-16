//
// Created by Admin on 14/03/2025.
//

#include <MyECS/SingletonsView.h>

using namespace My::MyECS;

CmptAccessPtr SingletonsView::GetSingleton(CmptAccessType t) const {
  for (size_t i = 0; i < num; i++) {
    if (singletons[i].AccessType() == t)
      return singletons[i];
  }
  return CmptAccessPtr::Invalid();
}
