//
// Created by Admin on 21/02/2025.
//

#pragma once

#include "SystemSchedule.inl"

#include <functional>

namespace My {
class ArchetypeMngr;

class SystemMngr {
 public:
  static SystemMngr& Instance() {
    static SystemMngr instance;
    return instance;
  }

  template <typename Cmpt>
  void Regist();

  void GenSchedule(SystemSchedule<SysType::OnStart>& schedule);
  void GenSchedule(SystemSchedule<SysType::OnUpdate>& schedule);
  void GenSchedule(SystemSchedule<SysType::OnStop>& schedule);

 private:
  std::vector<std::function<void(SystemSchedule<SysType::OnStart>&)>>
      staticStartScheduleFuncs;
  std::vector<std::function<void(SystemSchedule<SysType::OnUpdate>&)>>
      staticUpdateScheduleFuncs;
  std::vector<std::function<void(SystemSchedule<SysType::OnStop>&)>>
      staticStopScheduleFuncs;

  std::vector<std::function<void(SystemSchedule<SysType::OnStart>&)>>
      dynamicStartScheduleFuncs;
  std::vector<std::function<void(SystemSchedule<SysType::OnUpdate>&)>>
      dynamicUpdateScheduleFuncs;
  std::vector<std::function<void(SystemSchedule<SysType::OnStop>&)>>
      dynamicStopScheduleFuncs;

 private:
  SystemMngr() = default;
};
}  // namespace My

#include "SystemMngr.inl"
