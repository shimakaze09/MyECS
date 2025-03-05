//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "EntityMngr.h"
#include "SystemMngr.h"

#include <mutex>

namespace My {
class World {
 public:
  World() : schedule{this} {}

  SystemMngr systemMngr;
  EntityMngr entityMngr;

  // static OnUpdateSchedule
  // parallel OnUpdate
  // Commands, one-by-one
  void Update();

  std::string DumpUpdateJobGraph();

  void AddCommand(const std::function<void()>& command);

 private:
  mutable JobExecutor executor;
  Schedule schedule;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;

  // command
  std::vector<std::function<void()>> commandBuffer;
  std::mutex commandBufferMutex;
  void RunCommands();
};
}  // namespace My
