//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

using namespace My;
using namespace std;

World::World() : mngr{&sysMngr, this} {}

void World::Update(bool dump) {
  map<SystemMngr::ScheduleType, tf::Taskflow> type2tf;
  sysMngr.GenTaskflow(type2tf);
  for (auto& [type, taskflow] : type2tf) {
    if (dump)
      taskflow.dump(std::cout);
    executor.run(taskflow).wait();
  }
  mngr.RunCommand();
}
