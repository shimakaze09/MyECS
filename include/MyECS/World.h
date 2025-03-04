//
// Created by Admin on 26/12/2024.
//

#pragma once

#include "Entity.h"
#include "SystemMngr.h"

namespace My {
class World {
 public:
  SystemMngr systemMngr;

  World();

  template <typename... Cmpts>
  std::tuple<EntityPtr, Cmpts*...> CreateEntity();

  // static OnUpdateSchedule
  // parallel OnUpdate
  // Commands, one-by-one
  void Update();

  std::string DumpUpdateJobGraph();

  void AddCommand(const std::function<void()>& command);

 private:
  mutable JobExecutor executor;
  Schedule schedule;

  EntityMngr entityMngr;

  Job jobGraph;
  std::vector<Job*> jobs;
  Pool<Job> jobPool;
};
}  // namespace My

#include "detail/World.inl"
