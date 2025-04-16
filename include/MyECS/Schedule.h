#pragma once

#include <map>
#include <memory>
#include <memory_resource>

#include "details/Job.h"
#include "details/SystemFunc.h"

namespace My::MyECS::details {
struct Compiler;
}

namespace My::MyECS {
class EntityMngr;
class SystemMngr;
class SysFuncGraph;

// [description]
// system infomation record
// - SystemFunc
// - orders
// - dynamic filter changes
// [detail]
// schedule will be clear at the beginning of the next World::Update()
class Schedule {
 public:
  Schedule()
      : rsrc{std::make_unique<std::pmr::unsynchronized_pool_resource>()},
        sysFuncAllocator{rsrc.get()} {}

  // Func's argument list:
  // [const] World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // Entity
  // std::size_t indexInQuery
  // <tagged-components>: {LastFrame|Write|Latest}<Cmpt>...
  // CmptsView
  template <typename Func>
  const SystemFunc* RegisterEntityJob(Func&&, std::string name,
                                      bool isParallel = true,
                                      ArchetypeFilter = {}, CmptLocator = {},
                                      SingletonLocator = {},
                                      RandomAccessor = {});

  // Func's argument list:
  // [const] World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // ChunkView (necessary)
  template <typename Func>
  const SystemFunc* RegisterChunkJob(Func&&, std::string name,
                                     ArchetypeFilter = {},
                                     bool isParallel = true,
                                     SingletonLocator = {},
                                     RandomAccessor = {});

  // Func's argument list:
  // [const] World*
  // {LastFrame|Write|Latest}<Singleton<Cmpt>>
  // SingletonsView
  template <typename Func>
  const SystemFunc* RegisterJob(Func&&, std::string name, SingletonLocator = {},
                                RandomAccessor = {});

  void RegisterCommand(std::function<void(World*)> command, int layer = 0) {
    commandBuffer[layer].push_back(std::move(command));
  }

  Schedule& Order(std::string_view x, std::string_view y);

  Schedule& InsertNone(std::string_view sys, TypeID);
  Schedule& EraseNone(std::string_view sys, TypeID);

 private:
  template <typename... Args>
  const SystemFunc* Request(Args&&...);

  void Clear();

  struct CmptSysFuncs {
    std::vector<SystemFunc*> lastFrameSysFuncs;
    std::vector<SystemFunc*> writeSysFuncs;
    std::vector<SystemFunc*> latestSysFuncs;
  };
  friend struct details::Compiler;
  std::unordered_map<TypeID, CmptSysFuncs> GenCmptSysFuncsMap() const;

  SysFuncGraph GenSysFuncGraph() const;

  // SystemFunc's hashcode to pointer of SystemFunc
  std::unordered_map<std::size_t, SystemFunc*> sysFuncs;

  // SystemFunc's hashcode to SystemFunc's hashcode
  // parent to children
  std::unordered_map<std::size_t, std::size_t> sysFuncOrder;

  struct FilterChange {
    std::set<TypeID> insertNones;
    std::set<TypeID> eraseNones;
  };

  std::unordered_map<std::size_t, FilterChange> sysFilterChange;

  std::map<int, std::vector<std::function<void(World*)>>> commandBuffer;

  std::unique_ptr<std::pmr::unsynchronized_pool_resource> rsrc;
  std::pmr::polymorphic_allocator<SystemFunc> sysFuncAllocator;
  friend class World;
};
}  // namespace My::MyECS

#include "details/Schedule.inl"
