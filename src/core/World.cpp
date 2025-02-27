//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

using namespace My;
using namespace std;

World::World() : mngr{&sysMngr, this} {}

void World::Update(bool dump) {
  tf::Taskflow taskflow;
  if (dump)
    taskflow.dump(std::cout);
  executor.run(taskflow).wait();
  mngr.RunCommand();
}
