//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "EntityMngr.h"
#include "SystemMngr.h"

#include <MyGraphviz/MyGraphviz.h>

namespace My {
// SystemMngr + EntityMngr
class World {
 public:
  World() : schedule{&entityMngr, &systemMngr} {}

  SystemMngr systemMngr;
  EntityMngr entityMngr;

  // 1. schedule: run registered System's static OnUpdate(Schedule&)
  // 2. gen job graph: schedule -> graph
  // 3. run job graph in worker threads
  // 4. run commands in main thread
  void Update();

  // after running Update
  // you can use graphviz to vistualize the graph
  std::string DumpUpdateJobGraph() const;

  // after running Update
  // use RTDCmptTraits' registered component name
  Graphviz::Graph GenUpdateFrameGraph() const;

 private:
  mutable JobExecutor executor;
  Schedule schedule;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;
};
}  // namespace My
