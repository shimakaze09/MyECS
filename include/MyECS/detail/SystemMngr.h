//
// Created by Admin on 21/02/2025.
//

#pragma once

#include <MyDP/Basic/xSTL/xMap.h>
#include "../SystemSchedule.h"

#include <functional>

namespace My {
class SystemMngr {
 public:
  SystemMngr(ArchetypeMngr* archetypeMngr);

  template <typename Cmpt>
  void Regist();

  void GenStartTaskflow(tf::Taskflow& taskflow);
  void GenUpdateTaskflow(tf::Taskflow& taskflow);
  void GenStopTaskflow(tf::Taskflow& taskflow);

 private:
  std::unordered_set<size_t> registedCmptID;

  SystemSchedule startSchedule;
  SystemSchedule updateSchedule;
  SystemSchedule stopSchedule;

  std::vector<std::function<void(SystemSchedule&)>> staticStartScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> staticUpdateScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> staticStopScheduleFuncs;

  std::vector<std::function<void(SystemSchedule&)>> dynamicStartScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> dynamicUpdateScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> dynamicStopScheduleFuncs;
};
}  // namespace My

#include "SystemMngr.inl"
