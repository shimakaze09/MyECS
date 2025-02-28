//
// Created by Admin on 22/02/2025.
//

#pragma once

#include <MyTemplate/Func.h>
#include <_deps/nameof.hpp>

namespace My::detail::SystemSchedule_ {
template <typename Func>
struct GenSystem;
}  // namespace My::detail::SystemSchedule_

namespace My {
template <typename Cmpt, typename Func>
SystemSchedule& SystemSchedule::Regist(Func Cmpt::* func) {
  Regist(detail::SystemSchedule_::GenSystem<Func Cmpt::*>::run(func),
         nameof::nameof_type<Func Cmpt::*>());
  return *this;
}

template <typename Func>
SystemSchedule& SystemSchedule::Regist(Func&& func, std::string_view name) {
  detail::SystemSchedule_::
      Schedule<FuncTraits_ArgList<std::remove_reference_t<Func>>>::run(
          this, std::forward<Func>(func), name);
  return *this;
}
}  // namespace My

namespace My::detail::SystemSchedule_ {
template <typename... TagedCmpts>
struct Schedule<TypeList<TagedCmpts...>> {
  template <typename Func>
  static auto run(SystemSchedule* sysSchedule, Func&& func,
                  std::string_view name) noexcept {
    auto system = sysSchedule->RequestSystem(name);
    sysSchedule->mngr->GenTaskflow(system, func);
    if (!system->empty())
      (Regist<TagedCmpts>(sysSchedule->id2rw, system), ...);
  }

  template <typename TagedCmpt>
  static void Regist(
      std::unordered_map<size_t, SystemSchedule::RWSystems>& id2rw,
      tf::Taskflow* system) {
    using Cmpt = CmptTag::RemoveTag_t<TagedCmpt>;
    if constexpr (CmptTag::IsLastFrame_v<TagedCmpt>)
      id2rw[My::TypeID<Cmpt>].pre_readers.push_back(system);
    else if constexpr (CmptTag::IsWrite_v<TagedCmpt>)
      id2rw[My::TypeID<Cmpt>].writers.push_back(system);
    else
      id2rw[My::TypeID<Cmpt>].post_readers.push_back(system);
  }
};
}  // namespace My::detail::SystemSchedule_

namespace My::detail::SystemSchedule_ {
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
}  // namespace My::detail::SystemSchedule_
