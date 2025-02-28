//
// Created by Admin on 22/02/2025.
//

#pragma once

#include "detail/ArchetypeMngr.h"

#include <MyBL/Pool.h>

#include <taskflow/taskflow.hpp>

namespace My::detail::SystemSchedule_ {
template <typename ArgList>
struct Schedule;
}

namespace My {
using System = tf::Taskflow;

class SystemSchedule {
 public:
  SystemSchedule(ArchetypeMngr* mngr);
  ~SystemSchedule();

  template <typename Func>
  SystemSchedule& Regist(Func&& func, std::string_view name);

  template <typename Cmpt, typename Func>
  SystemSchedule& Regist(Func Cmpt::* func);

  // TODO: not parallel
  // template <typename Func>
  // SystemSchedule& RegistNotParallel(Func&& func, std::string_view name);
  //
  // template <typename Cmpt, typename Func>
  // SystemSchedule& RegistNotParallel(Func Cmpt::* func);

 private:
  friend class World;
  void Clear();

  bool GenTaskflow(tf::Taskflow& taskflow) const;

  struct RWSystems {
    std::vector<System*> writers;
    std::vector<System*> readers;
  };

  System* RequestSystem(std::string_view name);

  bool IsDAG() const noexcept;

  ArchetypeMngr* const mngr;
  std::vector<System*> requestedSysVec;
  std::unordered_map<size_t, RWSystems> id2rw;
  Pool<System> syspool;

  template <typename ArgList>
  friend struct detail::SystemSchedule_::Schedule;
};
}  // namespace My

#include "detail/SystemSchedule.inl"
