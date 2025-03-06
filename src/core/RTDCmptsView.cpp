//
// Created by Admin on 6/03/2025.
//

#include <MyECS/RTDCmptsView.h>

#include <MyECS/EntityLocator.h>

using namespace My;
using namespace std;

RTDCmptsView::CmptHandle::Mode RTDCmptsView::Iterator::GetMode() const {
  auto type = *typeIter;
  if (locator->LastFrameCmptTypes().find(type) !=
      locator->LastFrameCmptTypes().end())
    return CmptHandle::Mode::LAST_FRAME;
  else if (locator->WriteCmptTypes().find(type) !=
           locator->WriteCmptTypes().end())
    return CmptHandle::Mode::WRITE;
  else if (locator->LatestCmptTypes().find(type) !=
           locator->LatestCmptTypes().end())
    return CmptHandle::Mode::LATEST;
  else
    return CmptHandle::Mode::INVALID;
}

RTDCmptsView::Iterator RTDCmptsView::begin() const noexcept {
  return {locator, locator->CmptTypes().begin(), cmpts};
}

RTDCmptsView::Iterator RTDCmptsView::end() const noexcept {
  return {locator, locator->CmptTypes().end(),
          cmpts + locator->CmptTypes().size()};
}

const set<CmptType>& RTDCmptsView::CmptTypes() const noexcept {
  return locator->CmptTypes();
}
