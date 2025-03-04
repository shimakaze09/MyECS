//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

#include <MyECS/detail/CmptSysMngr.h>

using namespace My;
using namespace std;

World::World()
    : entityMngr{this},
      startRegistrar{&entityMngr},
      updateRegistrar{&entityMngr},
      stopRegistrar{&entityMngr} {}

void World::Start() {
  startRegistrar.schedule.Clear();
  startJobGraph.clear();

  CmptSysMngr::Instance().GenSchedule(startRegistrar, *this);
  startRegistrar.schedule.GenJobGraph(startJobGraph);

  executor.run(startJobGraph).wait();

  entityMngr.RunCommands();
}

void World::Update() {
  updateRegistrar.schedule.Clear();
  updateJobGraph.clear();

  CmptSysMngr::Instance().GenSchedule(updateRegistrar, *this);
  updateRegistrar.schedule.GenJobGraph(updateJobGraph);

  executor.run(updateJobGraph).wait();

  entityMngr.RunCommands();
}

void World::Stop() {
  stopRegistrar.schedule.Clear();
  stopJobGraph.clear();

  CmptSysMngr::Instance().GenSchedule(stopRegistrar, *this);
  stopRegistrar.schedule.GenJobGraph(stopJobGraph);

  executor.run(stopJobGraph).wait();

  entityMngr.RunCommands();
}

string World::DumpStartTaskflow() const {
  return startJobGraph.dump();
}

string World::DumpUpdateTaskflow() const {
  return updateJobGraph.dump();
}

string World::DumpStopTaskflow() const {
  return stopJobGraph.dump();
}

void World::AddCommand(const std::function<void()>& command) {
  entityMngr.AddCommand(command);
}
