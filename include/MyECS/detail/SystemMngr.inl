//
// Created by Admin on 22/02/2025.
//

#pragma once

#include <MyTemplate/Concept.h>
#include <MyTemplate/TypeID.h>
#include <MyTemplate/Typelist.h>

#include <cassert>

namespace My::detail::SystemMngr_ {
template <typename T>
Concept(HaveOnStart, &T::OnStart);

template <typename T>
Concept(HaveOnUpdate, &T::OnUpdate);

template <typename T>
Concept(HaveOnStop, &T::OnStop);

template <typename T>
Concept(HaveOnStartSchedule, &T::OnStartSchedule);

template <typename T>
Concept(HaveOnUpdateSchedule, &T::OnUpdateSchedule);

template <typename T>
Concept(HaveOnStopSchedule, &T::OnStopSchedule);
}  // namespace My::detail::SystemMngr_

namespace My {
template <typename Cmpt>
void SystemMngr::Regist() {
  if constexpr (Require<detail::SystemMngr_::HaveOnStart, Cmpt>) {
    staticStartScheduleFuncs.push_back(
        [](SystemSchedule& schedule) { schedule.Regist(&Cmpt::OnStart); });
  }
  if constexpr (Require<detail::SystemMngr_::HaveOnUpdate, Cmpt>) {
    staticUpdateScheduleFuncs.push_back(
        [](SystemSchedule& schedule) { schedule.Regist(&Cmpt::OnUpdate); });
  }
  if constexpr (Require<detail::SystemMngr_::HaveOnStop, Cmpt>) {
    staticStopScheduleFuncs.push_back(
        [](SystemSchedule& schedule) { schedule.Regist(&Cmpt::OnStop); });
  }

  if constexpr (Require<detail::SystemMngr_::HaveOnStartSchedule, Cmpt>)
    dynamicStartScheduleFuncs.push_back(&Cmpt::OnStartSchedule);
  if constexpr (Require<detail::SystemMngr_::HaveOnUpdateSchedule, Cmpt>)
    dynamicUpdateScheduleFuncs.push_back(&Cmpt::OnUpdateSchedule);
  if constexpr (Require<detail::SystemMngr_::HaveOnStopSchedule, Cmpt>)
    dynamicStopScheduleFuncs.push_back(&Cmpt::OnStopSchedule);
}
}  // namespace My
