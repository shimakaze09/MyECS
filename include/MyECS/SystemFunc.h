//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "Entity.h"
#include "EntityQuery.h"

#include <functional>

namespace My {
class SystemFunc {
 public:
  EntityQuery query;

  template <typename Func>
  SystemFunc(Func&& func, std::string name,
             EntityFilter filter = EntityFilter{});

  template <typename Func>
  SystemFunc(Func&& func, EntityFilter filter = EntityFilter{});

  const std::string& Name() const noexcept { return name; }

  static size_t HashCode(std::string_view name) {
    return std::hash<std::string_view>{}(name);
  }

  size_t HashCode() const noexcept { return hashCode; }

  void operator()(Entity e, void** cmptArr) { return func(e, cmptArr); }

 private:
  template <typename Func, typename ArgList>
  SystemFunc(Func&& func, std::string name, EntityFilter filter, ArgList);

  std::function<void(Entity, void**)> func;

  std::string name;
  size_t hashCode;  // after name
};
}  // namespace My

#include "detail/SystemFunc.inl"
