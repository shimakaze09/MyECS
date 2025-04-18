#include <MyECS/CmptsView.hpp>

using namespace Smkz::MyECS;

CmptAccessPtr CmptsView::GetCmpt(AccessTypeID t) const noexcept {
  for (const auto& cmpt : cmpts) {
    if (cmpt.AccessType() == t) return cmpt;
  }
  return CmptAccessPtr::Invalid();
}
