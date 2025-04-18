#include <MyECS/SingletonsView.hpp>

using namespace Smkz::MyECS;

CmptAccessPtr SingletonsView::GetSingleton(AccessTypeID t) const noexcept {
  for (const auto& singleton : singletons) {
    if (singleton.AccessType() == t) return singleton;
  }
  return CmptAccessPtr::Invalid();
}
