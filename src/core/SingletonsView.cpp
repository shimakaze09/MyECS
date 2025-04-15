#include <MyECS/SingletonsView.h>

using namespace My::MyECS;

CmptAccessPtr SingletonsView::GetSingleton(AccessTypeID t) const noexcept {
  for (const auto& singleton : singletons) {
    if (singleton.AccessType() == t)
      return singleton;
  }
  return CmptAccessPtr::Invalid();
}
