//
// Created by Admin on 22/02/2025.
//

#pragma once

#include <MyTemplate/Concept.h>
#include <MyTemplate/Func.h>
#include <MyTemplate/Typelist.h>

namespace My::detail::SystemMngr_ {
template <typename T>
Concept(HaveOnRegist, &T::OnRegist);

template <typename T>
Concept(HaveOnUpdate, &T::OnUpdate);

template <typename T>
Concept(HaveOnSchedule, &T::OnSchedule);

template <typename Cmpt>
struct GenUpdateSystem;
}  // namespace My::detail::SystemMngr_

namespace My {
template <typename Cmpt>
void SystemMngr::Regist() {
  if (registedCmptID.find(TypeID<Cmpt>) != registedCmptID.end())
    return;
  registedCmptID.insert(TypeID<Cmpt>);

  if constexpr (Require<detail::SystemMngr_::HaveOnRegist, Cmpt>) {
    static bool flag = false;
    if (!flag) {
      Cmpt::OnRegist();
      flag = false;
    }
  }
  if constexpr (Require<detail::SystemMngr_::HaveOnUpdate, Cmpt>) {
    staticScheduleFuncs.push_back(
        [](SystemSchedule& schedule) { schedule.Regist(&Cmpt::OnUpdate); });
  }
  if constexpr (Require<detail::SystemMngr_::HaveOnSchedule, Cmpt>)
    dynamicScheduleFuncs.push_back(&Cmpt::OnSchedule);
}
}  // namespace My

namespace My::detail::SystemMngr_ {
template <typename Cmpt, typename ArgList>
struct GenUpdateSystemHelper;

template <typename Cmpt, typename... Cmpts>
struct GenUpdateSystemHelper<Cmpt, TypeList<Cmpts...>> {
  static auto run() noexcept {
    if constexpr (FuncTraits<decltype(&Cmpt::OnUpdate)>::is_const) {
      return [](const Cmpt* cmpt, Cmpts... cmpts) {
        cmpt->OnUpdate(cmpts...);
      };
    } else {
      return [](Cmpt* cmpt, Cmpts... cmpts) {
        cmpt->OnUpdate(cmpts...);
      };
    }
  }
};

template <typename Cmpt>
struct GenUpdateSystem {
  static auto run() noexcept {
    return GenUpdateSystemHelper<
        Cmpt, FuncTraits_ArgList<decltype(&Cmpt::OnUpdate)>>::run();
  }
};
}  // namespace My::detail::SystemMngr_
