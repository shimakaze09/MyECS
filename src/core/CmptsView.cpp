//
// Created by Admin on 13/03/2025.
//

#include <MyECS/CmptsView.h>

using namespace My::MyECS;
using namespace std;

CmptAccessPtr CmptsView::GetCmpt(AccessTypeID t) const noexcept {
  for (const auto& cmpt : cmpts) {
    if (cmpt.AccessType() == t)
      return cmpt;
  }
  return CmptAccessPtr::Invalid();
}
