//
// Created by Admin on 6/03/2025.
//

#include <MyECS/RTDCmptViewer.h>

#include <MyECS/EntityLocator.h>

using namespace My;
using namespace std;

RTDCmptViewer::Iterator RTDCmptViewer::begin() const noexcept {
  return { locator->CmptTypes().begin(), cmpts };
}

RTDCmptViewer::Iterator RTDCmptViewer::end() const noexcept {
  return { locator->CmptTypes().end(), cmpts + locator->CmptTypes().size() };
}

const set<CmptType>& RTDCmptViewer::CmptTypes() const noexcept {
  return locator->CmptTypes();
}
