//
// Created by Admin on 4/03/2025.
//

#pragma once

#include "SystemFunc.h"
#include "detail/Job.h"
#include "detail/SysFuncGraph.h"

#include <MyContainer/Pool.h>

#include <map>

namespace My::MyECS::detail {
struct Compiler;
}  // namespace My::MyECS::detail

namespace My::MyECS {
class EntityMngr;
class SystemMngr;

// [description]
// system information record
// - SystemFunc
// - orders
// - dynamic filter changes
// [detail]
// schedule will be clear at the beginning of the next World::Update()
class Schedule {
 public:
  // Func's argument list:
  // World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // Entity
  // size_t indexInQuery
  // <tagged-components>: {LastFrame|Write|Latest}<Cmpt>...
  // CmptsView
  template <typename Func>
  const SystemFunc* RegisterEntityJob(Func&&, std::string name,
                                      ArchetypeFilter = {}, CmptLocator = {},
                                      SingletonLocator = {},
                                      bool isParallel = true);

  // Func's argument list:
  // World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // ChunkView (necessary)
  template <typename Func>
  const SystemFunc* RegisterChunkJob(Func&&, std::string name,
                                     ArchetypeFilter = {},
                                     SingletonLocator = {},
                                     bool isParallel = true);

  // Func's argument list:
  // World*
  // {LastFrame|Write|Latest}<Singleton<Cmpt>>
  // SingletonsView
  template <typename Func>
  const SystemFunc* RegisterJob(Func&&, std::string name,
                                SingletonLocator = {});

  Schedule& Order(std::string_view x, std::string_view y);

  Schedule& LockFilter(std::string_view sys);

  Schedule& InsertNone(std::string_view sys, CmptType);
  Schedule& EraseNone(std::string_view sys, CmptType);

 private:
  template <typename... Args>
  const SystemFunc* Request(Args&&...);

  void Clear();

  struct CmptSysFuncs {
    std::vector<SystemFunc*> lastFrameSysFuncs;
    std::vector<SystemFunc*> writeSysFuncs;
    std::vector<SystemFunc*> latestSysFuncs;
  };
  friend struct detail::Compiler;
  std::unordered_map<CmptType, CmptSysFuncs> GenCmptSysFuncsMap() const;

  SysFuncGraph GenSysFuncGraph() const;

  // SystemFunc's hashcode to pointer of SystemFunc
  std::unordered_map<size_t, SystemFunc*> sysFuncs;

  // SystemFunc's hashcode to SystemFunc's hashcode
  // parent to children
  std::unordered_map<size_t, size_t> sysFuncOrder;

  struct FilterChange {
    std::set<CmptType> insertNones;
    std::set<CmptType> eraseNones;
  };

  std::unordered_map<size_t, FilterChange> sysFilterChange;
  std::unordered_set<size_t> sysLockFilter;

  Pool<SystemFunc> sysFuncPool;
  friend class World;
};
}  // namespace My::MyECS

#include "detail/Schedule.inl"
