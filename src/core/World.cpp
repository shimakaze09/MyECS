//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

using namespace My;
using namespace std;

World::World() : sysMngr{&mngr}, mngr{&sysMngr, this} {}

void World::Start() {
  startTaskflow.clear();
  sysMngr.GenStartTaskflow(startTaskflow);
  executor.run(startTaskflow).wait();
  mngr.RunCommand();
}

void World::Update() {
  updateTaskflow.clear();
  sysMngr.GenUpdateTaskflow(updateTaskflow);
  executor.run(updateTaskflow).wait();
  mngr.RunCommand();
}

void World::Stop() {
  stopTaskflow.clear();
  sysMngr.GenStopTaskflow(stopTaskflow);
  executor.run(stopTaskflow).wait();
  mngr.RunCommand();
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
