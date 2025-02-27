//
// Created by Admin on 22/02/2025.
//

#include <MyECS/detail/SystemMngr.h>

using namespace My;
using namespace std;

SystemMngr::SystemMngr(ArchetypeMngr* archetypeMngr)
    : schedule(archetypeMngr) {}

void SystemMngr::GenTaskflow(tf::Taskflow& taskflow) {
  assert(taskflow.empty());

  schedule.Clear();

  for (auto& func : dynamicScheduleFuncs)
    func(schedule);

  for (const auto& func : staticScheduleFuncs)
    func(schedule);

  schedule.GenTaskflow(taskflow);
}
