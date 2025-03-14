//
// Created by Admin on 13/03/2025.
//

#include <MyECS/CmptsView.h>

using namespace My::MyECS;
using namespace std;

CmptPtr CmptsView::GetCmpt(CmptType t) const {
  for (size_t i = 0; i < num; i++) {
    if (cmpts[i].Type() == t) {
      assert(cmpts[i].Type().GetAccessMode() == t.GetAccessMode());
      return cmpts[i];
    }
  }
  return {CmptType::Invalid(), nullptr};
}
