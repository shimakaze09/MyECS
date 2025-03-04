//
// Created by Admin on 2/03/2025.
//

#pragma once

#include "SystemTraits.h"

#include <MyTemplate/Func.h>

namespace My::detail::ScheduleRegistrar_ {
template <typename Func>
struct GenSystem;

template <SysType type, typename... Args>
struct ScheduleAdd;
}  // namespace My::detail::ScheduleRegistrar_

namespace My {
template <SysType type>
template <typename Func>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::Register(
    const std::string& name, Func&& func) {
  detail::ScheduleRegistrar_::
      ScheduleAdd<type, FuncTraits_ArgList<std::remove_reference_t<Func>>>::run(
          mngr, schedule, std::forward<Func>(func), name);
  return *this;
}

template <SysType type>
template <typename Cmpt, typename Func>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::Register(
    const std::string& name, Func Cmpt::* func) {
  Register(name,
           detail::ScheduleRegistrar_::GenSystem<Func Cmpt::*>::run(func));
  return *this;
}

template <SysType type>
template <typename Cmpt>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::Register() {
  static_assert(
      HaveCmptSys<Cmpt, type>,
      "<Cmpt> have no corresponding System (OnStart/OnUpdate/OnStop)");
  Register(std::string(DefaultSysName<Cmpt, type>()), GetCmptSys<Cmpt, type>());
  return *this;
}

template <SysType type>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::Order(
    std::string_view first, const std::string& second) {
  auto target = schedule.sysOrderMap.find(first);
  if (target != schedule.sysOrderMap.end())
    target->second.insert(second);
  else
    schedule.sysOrderMap.emplace(std::string(first),
                                 std::set<std::string>{second});

  return *this;
}

template <SysType type>
template <typename CmptFirst, typename CmptSecond>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::Order() {
  return Order(DefaultSysName<CmptFirst, type>(),
               std::string(DefaultSysName<CmptSecond, type>()));
}

template <SysType type>
template <typename Cmpt>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::Before(
    std::string_view name) {
  return Order(name, std::string(DefaultSysName<Cmpt, type>()));
}

template <SysType type>
template <typename Cmpt>
ScheduleRegistrar<type>& ScheduleRegistrar<type>::After(
    const std::string& name) {
  return Order(DefaultSysName<Cmpt, type>(), name);
}
}  // namespace My

namespace My::detail::ScheduleRegistrar_ {
template <SysType type, typename... Args>
struct ScheduleAdd<type, TypeList<Args...>> {
  template <typename Sys>
  static auto run(EntityMngr* mngr, Schedule& schedule, Sys&& sys,
                  const std::string& name) noexcept {
    auto job = schedule.RequestJob(name);
    mngr->GenJob(job, sys);
    if (!job->empty())
      (Register<Args>(schedule, job), ...);
  }

  template <typename Arg>
  static void Register(Schedule& schedule, Job* job) {
    if constexpr (CmptTag::IsLastFrame_v<Arg>)
      schedule.id2rw[My::TypeID<CmptTag::RemoveTag_t<Arg>>]
          .pre_readers.push_back(job);
    else if constexpr (CmptTag::IsWrite_v<Arg>)
      schedule.id2rw[My::TypeID<CmptTag::RemoveTag_t<Arg>>].writers.insert(job);
    else if constexpr (CmptTag::IsNewest_v<Arg>)
      schedule.id2rw[My::TypeID<CmptTag::RemoveTag_t<Arg>>]
          .post_readers.push_back(job);
    else if constexpr (CmptTag::IsBefore_v<Arg>)
      RegisterBefore(schedule, job, typename Arg::CmptList{});
    else if constexpr (CmptTag::IsAfter_v<Arg>)
      RegisterAfter(schedule, job, typename Arg::CmptList{});
    else if constexpr (CmptTag::IsQuery_v<Arg>)
      ;  // do nothing
    else
      static_assert(false, "ERROR::ScheduleAdd: unknown <Arg>");
  }

  template <typename... Cmpts>
  static void RegisterBefore(Schedule& schedule, Job* job, TypeList<Cmpts...>) {
    (schedule.sysOrderMap[job->name()].insert(
         std::string(DefaultSysName<Cmpts, type>())),
     ...);
  }

  template <typename... Cmpts>
  static void RegisterAfter(Schedule& schedule, Job* job, TypeList<Cmpts...>) {
    (schedule.sysOrderMap[std::string(DefaultSysName<Cmpts, type>())].insert(
         job->name()),
     ...);
  }
};

template <typename Cmpt, typename ArgList>
struct GenSystemHelper;

template <typename Cmpt, typename... Cmpts>
struct GenSystemHelper<Cmpt, TypeList<Cmpts...>> {
  template <typename Func>
  static auto run(Func func) noexcept {
    if constexpr (FuncTraits<std::decay_t<Func>>::is_const) {
      return [func](const Cmpt* cmpt, Cmpts... cmpts) {
        (cmpt->*func)(cmpts...);
      };
    } else {
      return [func](Cmpt* cmpt, Cmpts... cmpts) {
        (cmpt->*func)(cmpts...);
      };
    }
  }
};

template <typename Func>
struct GenSystem {
  static auto run(Func func) noexcept {
    return GenSystemHelper<
        FuncTraits_Obj<std::decay_t<Func>>,
        FuncTraits_ArgList<std::decay_t<Func>>>::run(std::forward<Func>(func));
  }
};
}  // namespace My::detail::ScheduleRegistrar_
