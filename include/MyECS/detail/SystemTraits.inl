//
// Created by Admin on 2/03/2025.
//

#pragma once

#include <_deps/nameof.hpp>

namespace My {
template <typename Cmpt, SysType type>
constexpr auto GetCmptSys() noexcept {
  static_assert(HaveCmptSys<Cmpt, type>,
                "<Cmpt> has no corresponding system (OnStart/OnUpdate/OnStop)");

  if constexpr (type == SysType::OnStart)
    return &Cmpt::OnStart;
  else if constexpr (type == SysType::OnUpdate)
    return &Cmpt::OnUpdate;
  else  // if constexpr (type == SysType::Stop)
    return &Cmpt::OnStop;
}

template <typename Cmpt, SysType type>
constexpr std::string_view DefaultSysName() noexcept {
  static_assert(HaveCmptSys<Cmpt, type>,
                "<Cmpt> has no corresponding system (OnStart/OnUpdate/OnStop)");

  return nameof::nameof_type<decltype(GetCmptSys<Cmpt, type>())>();
}

template <typename System, SysType type>
constexpr ScheduleFunc<type>* GetSchedule() noexcept {
  static_assert(HaveSchedule<System, type>,
                "<System> has no corresponding schedule function: "
                "OnSchedule(SystemSchedule<SysType::...>&)");

  return MemFuncOf<ScheduleFunc<type>>::run(&System::OnSchedule);
}
}  // namespace My
