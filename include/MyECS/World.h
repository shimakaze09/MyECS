//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "EntityMngr.h"
#include "SystemMngr.h"

namespace My {
// SystemMngr + EntityMngr
class World {
 public:
  World() : schedule{&entityMngr, &systemMngr} {}

  SystemMngr systemMngr;
  EntityMngr entityMngr;

  // 1. schedule: run registered System's static OnUpdate(Schedule&)
  // 2. job graph: schedule -> graph
  // 3. run job graph in worker threads
  // 4. run commands in main threads
  void Update();

  // after running Update
  std::string DumpUpdateJobGraph() const;

 private:
  mutable JobExecutor executor;
  Schedule schedule;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;
};
}  // namespace My
