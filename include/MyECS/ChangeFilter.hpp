#pragma once

#include <MySmallFlat/small_flat_set.hpp>
#include <MyTemplate/Type.hpp>

namespace My::MyECS {
class ChangeFilter {
 public:
  small_flat_set<TypeID> types;
};
}  // namespace My::MyECS
