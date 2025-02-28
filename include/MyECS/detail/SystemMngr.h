//
// Created by Admin on 21/02/2025.
//

#pragma once

#include <functional>

namespace My {
class SystemSchedule;
class ArchetypeMngr;

class SystemMngr {
 public:
  static SystemMngr& Instance() {
    static SystemMngr instance;
    return instance;
  }

  template <typename Cmpt>
  void Regist();

  void GenStartSchedule(SystemSchedule& schedule);
  void GenUpdateSchedule(SystemSchedule& schedule);
  void GenStopSchedule(SystemSchedule& schedule);

 private:
  std::vector<std::function<void(SystemSchedule&)>> staticStartScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> staticUpdateScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> staticStopScheduleFuncs;

  std::vector<std::function<void(SystemSchedule&)>> dynamicStartScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> dynamicUpdateScheduleFuncs;
  std::vector<std::function<void(SystemSchedule&)>> dynamicStopScheduleFuncs;

 private:
  SystemMngr() = default;
};
}  // namespace My

#include "SystemMngr.inl"
