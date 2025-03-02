//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

#include <MyECS/detail/SystemMngr.h>

using namespace My;
using namespace std;

World::World()
    : mngr{this},
      startSchedule{&mngr},
      updateSchedule{&mngr},
      stopSchedule{&mngr} {}

void World::Start() {
  startSchedule.Clear();
  startTaskflow.clear();

  SystemMngr::Instance().GenSchedule(startSchedule);
  startSchedule.GenTaskflow(startTaskflow);

  executor.run(startTaskflow).wait();

  mngr.RunCommands();
}

void World::Update() {
  updateSchedule.Clear();
  updateTaskflow.clear();

  SystemMngr::Instance().GenSchedule(updateSchedule);
  updateSchedule.GenTaskflow(updateTaskflow);

  executor.run(updateTaskflow).wait();

  mngr.RunCommands();
}

void World::Stop() {
  stopSchedule.Clear();
  stopTaskflow.clear();

  SystemMngr::Instance().GenSchedule(stopSchedule);
  stopSchedule.GenTaskflow(stopTaskflow);

  executor.run(stopTaskflow).wait();

  mngr.RunCommands();
}

string World::DumpStartTaskflow() const {
  return startTaskflow.dump();
}

string World::DumpUpdateTaskflow() const {
  return updateTaskflow.dump();
}

string World::DumpStopTaskflow() const {
  return stopTaskflow.dump();
}

void World::AddCommand(const std::function<void()>& command) {
  mngr.AddCommand(command);
}
