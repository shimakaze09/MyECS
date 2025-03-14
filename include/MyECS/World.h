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

  // after running Update
  // you can use graphviz to visualize the graph
  std::string DumpUpdateJobGraph() const;

  // after running Update
  // use RTDCmptTraits' registered component name
  MyGraphviz::Graph GenUpdateFrameGraph() const;

  void Accept(IListener*) const;

  void AddCommand(std::function<void(World*)> command);

 private:
  mutable JobExecutor executor;
  Schedule schedule;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;

  // command
  std::vector<std::function<void(World*)>> commandBuffer;
  std::mutex commandBufferMutex;
  void RunCommands();

  // ==================================================
  World(const World& world) = delete;
  World(World&& world) = delete;
  World& operator==(World&& world) = delete;
  World& operator=(const World& world) = delete;
};
}  // namespace My::MyECS
