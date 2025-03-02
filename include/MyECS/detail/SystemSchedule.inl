//
// Created by Admin on 22/02/2025.
//

#pragma once

#include <MyTemplate/Func.h>

#include "../SystemSchedule.h"

namespace My::detail::SystemSchedule_ {
template <typename Func>
struct GenSystem;
}  // namespace My::detail::SystemSchedule_

namespace My {
template <SysType type>
template <typename Func>
SystemSchedule<type>& SystemSchedule<type>::Regist(Func&& func,
                                                   std::string_view name) {
  detail::SystemSchedule_::
      Schedule<type, FuncTraits_ArgList<std::remove_reference_t<Func>>>::run(
          this, std::forward<Func>(func), name);
  return *this;
}

template <SysType type>
template <typename Cmpt, typename Func>
SystemSchedule<type>& SystemSchedule<type>::Regist(Func Cmpt::* func) {
  Regist(detail::SystemSchedule_::GenSystem<Func Cmpt::*>::run(func),
         nameof::nameof_type<Func Cmpt::*>());
  return *this;
}

template <SysType type>
template <typename Cmpt, typename>
SystemSchedule<type>& SystemSchedule<type>::Regist() {
  Regist(GetSys<Cmpt, type>());
  return *this;
}

// ==============================================================================================

template <SysType type>
System* SystemSchedule<type>::RequestSystem(std::string_view name) {
  std::string sname(name);
  System* sys = syspool.Request(sname);
  systems[sname] = sys;
  return sys;
}

template <SysType type>
SystemSchedule<type>::SystemSchedule(ArchetypeMngr* mngr) : mngr{mngr} {}

template <SysType type>
SystemSchedule<type>::~SystemSchedule() {
  Clear();
}

template <SysType type>
void SystemSchedule<type>::Clear() {
  for (auto [name, sys] : systems)
    syspool.Recycle(sys);
  systems.clear();
  id2rw.clear();
  sysOrderMap.clear();
}

template <SysType type>
bool SystemSchedule<type>::GenTaskflow(tf::Taskflow& taskflow) const {
  if (!IsDAG())
    return false;

  std::unordered_map<System*, tf::Task> sys2task;

  for (auto& [ID, rw] : id2rw) {
    // toposort
    auto writers = rw.writers;           // copy
    std::vector<System*> sortedSystems;  // last to first
    while (!writers.empty()) {
      auto iter_begin = writers.begin();
      std::deque<System*> childrenQueue;  // FIFO, push_back, pop_front
      childrenQueue.push_back(*iter_begin);

      std::vector<System*> waitedSystems;

      while (!childrenQueue.empty()) {
        auto writer = childrenQueue.front();
        childrenQueue.pop_front();

        if (writers.find(writer) == writers.end())
          continue;

        waitedSystems.push_back(writer);
        writers.erase(writer);  // directly change writers

        auto target_children = sysOrderMap.find(writer->name());
        if (target_children == sysOrderMap.end())
          continue;

        for (const auto& sys_name : target_children->second)
          childrenQueue.push_back(systems.find(sys_name)->second);
      }

      for (auto iter = waitedSystems.rbegin(); iter != waitedSystems.rend();
           ++iter)
        sortedSystems.push_back(*iter);
    }

    std::vector<tf::Task> writerTasks;
    for (size_t i = 0; i < sortedSystems.size(); i++) {
      tf::Task task;

      auto target = sys2task.find(sortedSystems[i]);
      if (target == sys2task.end()) {
        task = taskflow.composed_of(*sortedSystems[i]);
        sys2task[sortedSystems[i]] = task;
      } else
        task = target->second;

      if (!writerTasks.empty())
        task.precede(writerTasks.back());

      writerTasks.push_back(task);
    }

    for (auto pre_reader : rw.pre_readers) {
      tf::Task task;
      auto target = sys2task.find(pre_reader);
      if (target == sys2task.end()) {
        task = taskflow.composed_of(*pre_reader);
        sys2task[pre_reader] = task;
      } else
        task = target->second;

      if (!writerTasks.empty())
        task.precede(writerTasks.back());
    }

    for (auto post_reader : rw.post_readers) {
      tf::Task task;
      auto target = sys2task.find(post_reader);
      if (target == sys2task.end()) {
        task = taskflow.composed_of(*post_reader);
        sys2task[post_reader] = task;
      } else
        task = target->second;

      if (!writerTasks.empty())
        task.succeed(writerTasks.front());
    }
  }

  return true;
}

template <SysType type>
bool SystemSchedule<type>::IsDAG() const noexcept {
  std::map<System*, std::set<System*>> graph;
  for (const auto& [ID, rw] : id2rw) {
    for (const auto& writer : rw.writers) {
      for (auto post_reader : rw.post_readers) {
        if (post_reader != writer)
          graph[writer].insert(post_reader);
      }
      for (auto pre_reader : rw.pre_readers) {
        if (pre_reader != writer)
          graph[pre_reader].insert(writer);
      }
    }
  }

  std::set<System*> visited;
  for (const auto& [parent, children] : graph) {
    if (visited.find(parent) != visited.end())
      continue;
    std::set<System*> sysSet;
    sysSet.insert(parent);
    std::set<System*> curVisited;
    while (!sysSet.empty()) {
      auto curTarget = sysSet.begin();
      auto curSys = *curTarget;
      sysSet.erase(curTarget);
      if (curVisited.find(curSys) != curVisited.end())
        return false;
      if (visited.find(curSys) != visited.end())
        continue;
      curVisited.insert(curSys);
      visited.insert(curSys);
      auto target = graph.find(curSys);
      if (target != graph.end()) {
        for (const auto& child : target->second)
          sysSet.insert(child);
      }
    }
  }
  return true;
}
}  // namespace My

namespace My::detail::SystemSchedule_ {
template <SysType type, typename... TagedCmpts>
struct Schedule<type, TypeList<TagedCmpts...>> {
  template <typename Func>
  static auto run(SystemSchedule<type>* sysSchedule, Func&& func,
                  std::string_view name) noexcept {
    auto system = sysSchedule->RequestSystem(name);
    sysSchedule->mngr->GenTaskflow(system, func);
    if (!system->empty())
      (Regist<TagedCmpts>(sysSchedule, system), ...);
  }

  template <typename TagedCmpt>
  static void Regist(SystemSchedule<type>* sysSchedule, tf::Taskflow* system) {
    using Cmpt = CmptTag::RemoveTag_t<TagedCmpt>;
    if constexpr (CmptTag::IsLastFrame_v<TagedCmpt>)
      sysSchedule->id2rw[My::TypeID<Cmpt>].pre_readers.push_back(system);
    else if constexpr (CmptTag::IsWrite_v<TagedCmpt>)
      sysSchedule->id2rw[My::TypeID<Cmpt>].writers.insert(system);
    else if constexpr (CmptTag::IsNewest_v<TagedCmpt>)
      sysSchedule->id2rw[My::TypeID<Cmpt>].post_readers.push_back(system);
    else if constexpr (CmptTag::IsBefore_v<TagedCmpt>)
      RegistBefore(sysSchedule, system, typename TagedCmpt::CmptList{});
    else  // if constexpr (CmptTag::IsAfter_v<TagedCmpt>)
      RegistAfter(sysSchedule, system, typename TagedCmpt::CmptList{});
  }

  template <typename... Cmpts>
  static void RegistBefore(SystemSchedule<type>* sysSchedule,
                           tf::Taskflow* system, TypeList<Cmpts...>) {
    (sysSchedule->sysOrderMap[system->name()].insert(
         std::string(DefaultSysName<Cmpts, type>())),
     ...);
  }

  template <typename... Cmpts>
  static void RegistAfter(SystemSchedule<type>* sysSchedule,
                          tf::Taskflow* system, TypeList<Cmpts...>) {
    (sysSchedule->sysOrderMap[std::string(DefaultSysName<Cmpts, type>())]
         .insert(system->name()),
     ...);
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
