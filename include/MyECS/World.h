//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "EntityMngr.h"
#include "SystemMngr.h"

#include <MyGraphviz/MyGraphviz.h>

#include <mutex>

namespace My::MyECS {
class IListener;

// SystemMngr + EntityMngr
class World {
 public:
  World();

  SystemMngr systemMngr;
  EntityMngr entityMngr;

  // 1. schedule: run registered System's static OnUpdate(Schedule&)
  // 2. gen job graph: schedule -> graph
  // 3. run job graph in worker threads
  // 4. run commands in main thread
  void Update();

  void AddCommand(std::function<void(World*)> command);

  // after running Update
  // you can use graphviz to vistualize the graph
  std::string DumpUpdateJobGraph() const;

  // after running Update
  // use RTDCmptTraits' registered component name
  MyGraphviz::Graph GenUpdateFrameGraph() const;

  void Accept(IListener*) const;

  // you can't run several parallel jobs in parallel because there is only an executor
  // you can't run parallel jobs in running job graph

  // Func's argument list:
  // World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // Entity
  // size_t indexInQuery
  // <tagged-components>: [const] <Cmpt>*...
  // CmptsView
  template <typename Func>
  void RunEntityJob(Func&&, bool isParallel = true, ArchetypeFilter = {},
                    CmptLocator = {}, SingletonLocator = {});

  // Func's argument list:
  // World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // ChunkView (necessary)
  template <typename Func>
  void RunChunkJob(Func&&, ArchetypeFilter = {}, bool isParallel = true,
                   SingletonLocator = {});

  // Func's argument list:
  // World*
  // {LastFrame|Write|Latest}<Singleton<Cmpt>>
  // SingletonsView
  template <typename Func>
  void RunJob(Func&&, SingletonLocator = {});

 private:
  bool inRunningJobGraph{false};

  mutable JobExecutor executor;
  Schedule schedule;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;

  // command
  std::vector<std::function<void(World*)>> commandBuffer;
  std::mutex commandBufferMutex;
  void RunCommands();

  void Run(SystemFunc*);

  // ==================================================
  World(const World& world) = delete;
  World(World&& world) = delete;
  World& operator==(World&& world) = delete;
  World& operator=(const World& world) = delete;
};
}  // namespace My::MyECS

#include "detail/World.inl"
