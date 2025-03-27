//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "../ChunkView.h"
#include "../CmptsView.h"
#include "../Entity.h"
#include "../EntityQuery.h"
#include "../SingletonLocator.h"
#include "../SingletonsView.h"

#include <functional>

namespace My::MyECS {
// [- description]
// system function registered by Schedule in <System>::OnUpdate(Schedule&)
// name + query(archetype filter + component locator) + singleton locator + function<...>
// name('s hashcode) must be unique in global
// query.filter.none can be change dynamically by other <System> with <Schedule>
// [- system function kind] (distinguish by argument list)
// common arguments : [const] World*, SingletonsView, {LastFrame|Latest}<Singleton<Cmpt>>
// 1. Mode::Entity: per entity function
// * Entity
// * size_t indexInQuery
// * <tagged-components>: {LastFrame|Write|Latest}<Cmpt>...
// * CmptsView
// 2. Mode::Chunk
// * size_t entityBeginIndexInQuery
// * ChunkView (necessary)
// 3. Mode::Job
// * Write<Singleton<Cmpt>> (only job can write singletons)
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
             SingletonLocator, bool isParallel);

  // Mode::Chunk
  template <typename Func>
  SystemFunc(Func&&, std::string name, ArchetypeFilter, SingletonLocator,
             bool isParallel);

  // Mode::Job
  template <typename Func>
  SystemFunc(Func&&, std::string name, SingletonLocator);

  const std::string& Name() const noexcept { return name; }

  static constexpr size_t HashCode(std::string_view name) noexcept {
    return hash_string(name);
  }

  size_t HashCode() const noexcept { return hashCode; }

  void operator()(World*, SingletonsView, Entity, size_t entityIndexInQuery,
                  CmptsView) const;
  void operator()(World*, SingletonsView, size_t entityBeginIndexInQuery,
                  ChunkView) const;
  void operator()(World*, SingletonsView) const;

  Mode GetMode() const noexcept { return mode; }

  bool IsParallel() const noexcept { return isParallel; }

  bool operator==(const SystemFunc& sysFunc) const noexcept {
    return name == sysFunc.name;
  }

 private:
  Mode mode;
  std::string name;
  size_t hashCode;  // after name
  bool isParallel;
  std::function<void(World*, SingletonsView, Entity, size_t, CmptsView,
                     ChunkView)>
      func;
};
}  // namespace My::MyECS

#include "SystemFunc.inl"
