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

  void GenTaskflow(tf::Taskflow& taskflow);

 private:
  std::unordered_set<size_t> registedCmptID;
  SystemSchedule schedule;
  std::vector<std::function<void(SystemSchedule&)>> staticScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> dynamicScheduleFuncs;
};
}  // namespace My

#include "SystemMngr.inl"
