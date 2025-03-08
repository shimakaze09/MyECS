//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "CmptTag.inl"

#include <MyContainer/Algorithm.h>

#include <MyTemplate/Func.h>

namespace My {
template <typename TaggedCmptList>
EntityLocator::EntityLocator(TaggedCmptList)
    : EntityLocator{Filter_t<TaggedCmptList, CmptTag::IsLastFrame>{},
                    Filter_t<TaggedCmptList, CmptTag::IsWrite>{},
                    Filter_t<TaggedCmptList, CmptTag::IsLatest>{}} {}

template <typename... LastFrameCmpts, typename... WriteCmpts,
          typename... LatestCmpts>
EntityLocator::EntityLocator(TypeList<LastFrameCmpts...>,
                             TypeList<WriteCmpts...>, TypeList<LatestCmpts...>)
    : lastFrameCmptTypes{CmptType::Of<CmptTag::RemoveTag_t<LastFrameCmpts>>...},
      writeCmptTypes{CmptType::Of<CmptTag::RemoveTag_t<WriteCmpts>>...},
      latestCmptTypes{CmptType::Of<CmptTag::RemoveTag_t<LatestCmpts>>...},
      cmptTypes{CmptType::Of<CmptTag::RemoveTag_t<LastFrameCmpts>>...,
                CmptType::Of<CmptTag::RemoveTag_t<WriteCmpts>>...,
                CmptType::Of<CmptTag::RemoveTag_t<LatestCmpts>>...},
      hashCode{GenHashCode()} {}
}  // namespace My
