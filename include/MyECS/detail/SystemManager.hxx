//
// Created by Admin on 19/12/2024.
//

#pragma once

#include "ArchetypeManager.hxx"

#include <taskflow/taskflow.hxx>

#include <MyTemplate/Concept.hxx>
#include <MyTemplate/Func.hxx>
#include <MyTemplate/TypeList.hxx>

#include <unordered_map>

namespace My::detail::SystemManager_ {
template <typename T>
Concept(HaveOnUpdate, &T::OnUpdate);

template <typename Cmpt>
struct Schedule;
}  // namespace My::detail::SystemManager_

namespace My {
class SystemManager {
 public:
  using System = tf::Taskflow;

  struct RWSystems {
    System* writer{nullptr};
    std::vector<System*> readers;
  };

  struct Table {
    System finalSys;
    std::unordered_map<size_t, System> tasks;
    std::unordered_map<size_t, RWSystems> id2rw;
  };

  void GenTaskflow(Table& table) const;

  template <typename Cmpt>
  void Regist(ArchetypeManager* mngr) {
    if constexpr (Require<detail::SystemManager_::HaveOnUpdate, Cmpt>) {
      if (schedules.find(TypeID<Cmpt>) == schedules.end()) {
        schedules[TypeID<Cmpt>] =
            detail::SystemManager_::Schedule<Cmpt>::run(mngr);
      }
    }
  }

 private:
  static bool IsDAG(const Table& table);

 private:
  std::unordered_map<size_t, std::function<void(Table& table)>> schedules;
};
}  // namespace My

namespace My::detail::SystemManager_ {
template <typename Cmpt, typename ArgList>
struct ScheduleHelper;

template <typename Cmpt, typename... Cmpts>
struct ScheduleHelper<Cmpt, TypeList<Cmpts...>> {
  static auto run(ArchetypeManager* mngr) noexcept {
    return [mngr](SystemManager::Table& table) {
      // schedule
      auto taskflow = &table.tasks[My::TypeID<Cmpt>];
      if constexpr (FuncTraits<decltype(&Cmpt::OnUpdate)>::is_const) {
        mngr->GenTaskflow(*taskflow, [](const Cmpt* cmpt, Cmpts... cmpts) {
          cmpt->OnUpdate(cmpts...);
        });
        Regist<const Cmpt>(table, taskflow);
      } else {
        mngr->GenTaskflow(*taskflow, [](Cmpt* cmpt, Cmpts... cmpts) {
          cmpt->OnUpdate(cmpts...);
        });
        Regist<Cmpt>(table, taskflow);
      }
      (Regist<std::remove_pointer_t<Cmpts>>(table, taskflow), ...);
    };
  }

  template <typename Cmpt>
  static void Regist(SystemManager::Table& table, tf::Taskflow* taskflow) {
    if constexpr (std::is_const_v<Cmpt>) {
      using RawCmpt = std::remove_const_t<Cmpt>;
      table.id2rw[My::TypeID<RawCmpt>].readers.push_back(taskflow);
    } else {
      assert(table.id2rw[My::TypeID<Cmpt>].writer == nullptr &&
             "two component write same component");
      table.id2rw[My::TypeID<Cmpt>].writer = taskflow;
    }
  }
};

template <typename Cmpt>
struct Schedule {
  static_assert(Require<HaveOnUpdate, Cmpt>);

  static auto run(ArchetypeManager* mngr) noexcept {
    return ScheduleHelper<
        Cmpt, FuncTraits_ArgList<decltype(&Cmpt::OnUpdate)>>::run(mngr);
  }
};
}  // namespace My::detail::SystemManager_