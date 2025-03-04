//
// Created by Admin on 4/03/2025.
//

#include <MyECS/EntityQuery.h>

#include <MyECS/detail/Util.h>

using namespace My;

EntityQuery::EntityQuery(EntityFilter filter, EntityLocator locator)
    : filter{std::move(filter)},
      locator{std::move(locator)},
      hashCode{hash_combine(filter.HashCode(), locator.HashCode())} {}
