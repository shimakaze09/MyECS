//
// Created by Admin on 13/03/2025.
//

#include <MyECS/CmptsView.h>
 
 #include <MyECS/EntityLocator.h>
 
 using namespace My::MyECS;
using namespace std;
 
CmptPtr CmptsView::GetCmpt(CmptType t) const {
  size_t i = 0;
  for (auto iter = locator->CmptTypes().begin(); iter != locator->CmptTypes().end(); ++iter, ++i) {
    if (*iter == t) {
      assert(iter->GetAccessMode() == t.GetAccessMode());
      return CmptPtr(*iter, *(cmpts + i));
    }
  }
  assert(false);
  return CmptPtr(t, nullptr);
}
