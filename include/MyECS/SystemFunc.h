//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "ChunkView.h"
#include "CmptsView.h"
#include "Entity.h"
#include "EntityQuery.h"
#include "SingletonLocator.h"
#include "SingletonsView.h"

#include <functional>

namespace My::MyECS {
// [description]
// system function registered by Schedule in <System>::OnUpdate(Schedule&)
// name + query + function<...>
// name must be unique in global
// query.filter can be change dynamically by other <System> with Schedule
// [system function kind] (distinguish by argument list)
// common : [World*], [{LastFrame|Write|Latest}Singleton<Component>], [SingletonsView]
// 1. per entity function
// * [Entity]
// * [size_t indexInQuery]
// * <tagged-component>: {LastFrame|Write|Latest}<Component>
// * [CmptsView]
// 2. chunk: ChunkView
// 3. job
class SystemFunc {
 public:
  enum class Mode {
    Entity,
    Chunk,
    Job,
  };

  EntityQuery entityQuery;
  SingletonLocator singletonLocator;

  // Mode::Entity
  template <typename Func>
  SystemFunc(Func&&, std::string name, ArchetypeFilter, CmptLocator,
             SingletonLocator);

  // Mode::Chunk
  template <typename Func>
  SystemFunc(Func&&, std::string name, ArchetypeFilter, SingletonLocator);

  // Mode::Job
  template <typename Func>
  SystemFunc(Func&&, std::string name, SingletonLocator);

  const std::string& Name() const noexcept { return name; }

  static constexpr size_t HashCode(std::string_view name) {
    return hash_string(name);
  }

  size_t HashCode() const noexcept { return hashCode; }

  void operator()(World*, SingletonsView, Entity, size_t entityIndexInQuery,
                  CmptsView);
  void operator()(World*, SingletonsView, ChunkView);
  void operator()(World*, SingletonsView);

  Mode GetMode() const noexcept { return mode; }

  bool operator==(const SystemFunc& sysFunc) const noexcept {
    return name == sysFunc.name;
  }

 private:
  std::function<void(World*, SingletonsView, Entity, size_t, CmptsView,
                     ChunkView)>
      func;

  std::string name;
  Mode mode;
  size_t hashCode;  // after name
};
}  // namespace My::MyECS

#include "detail/SystemFunc.inl"
