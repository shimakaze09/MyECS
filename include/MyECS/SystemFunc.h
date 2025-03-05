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

  static constexpr size_t HashCode(std::string_view name) {
    return RuntimeTypeID(name);
  }

  size_t HashCode() const noexcept { return hashCode; }

  void operator()(Entity e, size_t entityIndexInQuery, void** cmptArr) {
    return func(e, entityIndexInQuery, cmptArr);
  }

  // no arguments <Func>
  bool IsJob() const noexcept { return isJob; }

  bool operator==(const SystemFunc& func) const noexcept {
    return name == func.name;
  }

 private:
  template <typename Func, typename ArgList>
  SystemFunc(Func&& func, std::string name, EntityFilter filter, ArgList);

  std::function<void(Entity, size_t, void**)> func;

  std::string name;
  bool isJob;
  size_t hashCode;  // after name
};
}  // namespace My

#include "detail/SystemFunc.inl"
