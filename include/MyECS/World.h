//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "EntityMngr.h"
#include "SystemMngr.h"

namespace My {
class World {
 public:
  World() : schedule{&entityMngr, &systemMngr} {}

  SystemMngr systemMngr;
  EntityMngr entityMngr;

  // static OnUpdateSchedule
  // parallel OnUpdate
  // Commands, one-by-one
  void Update();

  std::string DumpUpdateJobGraph() const;

 private:
  mutable JobExecutor executor;
  Schedule schedule;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;
};
}  // namespace My
