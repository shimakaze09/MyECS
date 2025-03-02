//
// Created by Admin on 2/03/2025.
//

#pragma once

#include "SystemMngr.h"
#include "SystemSchedule.inl"

#include <functional>

namespace My {
class ArchetypeMngr;

class CmptSysMngr {
 public:
  static CmptSysMngr& Instance() {
    static CmptSysMngr instance;
    return instance;
  }

  template <typename Cmpt>
  void Register();

  void GenSchedule(SystemSchedule<SysType::OnStart>& schedule,
                   const SystemMngr& sysMngr);
  void GenSchedule(SystemSchedule<SysType::OnUpdate>& schedule,
                   const SystemMngr& sysMngr);
  void GenSchedule(SystemSchedule<SysType::OnStop>& schedule,
                   const SystemMngr& sysMngr);

 private:
  std::vector<std::function<ScheduleFunc<SysType::OnStart>>>
      staticStartScheduleFuncs;
  std::vector<std::function<ScheduleFunc<SysType::OnUpdate>>>
      staticUpdateScheduleFuncs;
  std::vector<std::function<ScheduleFunc<SysType::OnStop>>>
      staticStopScheduleFuncs;

  std::vector<ScheduleFunc<SysType::OnStart>*> dynamicStartScheduleFuncs;
  std::vector<ScheduleFunc<SysType::OnUpdate>*> dynamicUpdateScheduleFuncs;
  std::vector<ScheduleFunc<SysType::OnStop>*> dynamicStopScheduleFuncs;

 private:
  CmptSysMngr() = default;
};
}  // namespace My

#include "CmptSysMngr.inl"
