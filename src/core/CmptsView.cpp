//
// Created by Admin on 13/03/2025.
//

#include <MyECS/CmptsView.h>

using namespace My::MyECS;
using namespace std;

CmptAccessPtr CmptsView::GetCmpt(CmptAccessType t) const noexcept {
  for (size_t i = 0; i < num; i++) {
    if (cmpts[i].AccessType() == t) {
      return cmpts[i];
    }
  }
  return CmptAccessPtr::Invalid();
}
