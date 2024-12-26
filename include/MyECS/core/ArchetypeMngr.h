//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Archetype.h"

#include <MyTemplate/TypeList.h>

namespace My {
class World;
}

namespace My::detail {
class ArcheTypeMngr {
 public:
  ArcheTypeMngr(World* w) : w(w) {}

  inline World* World() const noexcept { return w; }

  template <typename... Cmpts>
  static constexpr size_t HashOf() noexcept {
    return My::TypeID<QuickSort_t<TypeList<Cmpts...>, My::TypeID_Less>>;
  }

  inline ArcheType& GetArcheTypeOf(size_t ArcheTypeHash) {
    auto target = map.find(ArcheTypeHash);
    assert(target != map.end());
    return target->second;
  }

  template <typename... Cmpts>
  inline ArcheType& GetArcheTypeOf() {
    constexpr size_t ArcheTypeHash =
        My::TypeID<QuickSort_t<TypeList<Cmpts...>, My::TypeID_Less>>;
    auto target = map.find(ArcheTypeHash);
    if (target == map.end()) {
      ArcheType& c = map[ArcheTypeHash];
      c.Init<Cmpts...>();
      return c;
    } else
      return target->second;
  }

  template <typename... Cmpts>
  const std::vector<ArcheType*> GetArcheTypeWith() {
    std::vector<ArcheType*> rst;
    if constexpr (sizeof...(Cmpts) == 0) {
      for (auto& p : map)
        rst.push_back(&p.second);
      return rst;
    } else {
      for (auto& p : map) {
        if (p.second.IsContain<Cmpts...>())
          rst.push_back(&p.second);
      }
      return rst;
    }
  }

 private:
  My::World* w;
  std::map<size_t, ArcheType> map;
};
}  // namespace My::detail
