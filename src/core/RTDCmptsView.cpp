//
// Created by Admin on 6/03/2025.
//

#include <MyECS/RTDCmptsView.h>

#include <MyECS/EntityLocator.h>

using namespace My::MyECS;
using namespace std;

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

RTDCmptsView::CmptHandle RTDCmptsView::Iterator::operator*() const {
  return {*typeIter, *ptr_cmpt, locator->GetCmptTagMode(*typeIter)};
}

const RTDCmptsView::CmptHandle* RTDCmptsView::Iterator::operator->()
    const noexcept {
  handle = {*typeIter, *ptr_cmpt, locator->GetCmptTagMode(*typeIter)};
  return &handle;
}
