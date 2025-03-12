//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptTag.inl"

#include <MyContainer/Algorithm.h>

#include <MyTemplate/Func.h>

namespace My::MyECS {
template <typename TaggedCmptList>
EntityLocator::EntityLocator(TaggedCmptList)
    : EntityLocator{Filter_t<TaggedCmptList, IsLastFrame>{},
                    Filter_t<TaggedCmptList, IsWrite>{},
                    Filter_t<TaggedCmptList, IsLatest>{}} {}

template <typename... LastFrameCmpts, typename... WriteCmpts,
          typename... LatestCmpts>
EntityLocator::EntityLocator(TypeList<LastFrameCmpts...>,
                             TypeList<WriteCmpts...>, TypeList<LatestCmpts...>)
    : lastFrameCmptTypes{CmptType::Of<RemoveTag_t<LastFrameCmpts>>...},
      writeCmptTypes{CmptType::Of<RemoveTag_t<WriteCmpts>>...},
      latestCmptTypes{CmptType::Of<RemoveTag_t<LatestCmpts>>...},
      cmptTypes{CmptType::Of<RemoveTag_t<LastFrameCmpts>>...,
                CmptType::Of<RemoveTag_t<WriteCmpts>>...,
                CmptType::Of<RemoveTag_t<LatestCmpts>>...},
      hashCode{GenHashCode()} {}
}  // namespace My::MyECS
