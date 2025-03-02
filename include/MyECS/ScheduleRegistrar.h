//
// Created by Admin on 2/03/2025.
//

#pragma once

#include "detail/ArchetypeMngr.h"
#include "detail/Schedule.h"
#include "detail/SystemTraits.h"

namespace My {
template <SysType type>
class ScheduleRegistrar {
 public:
  template <typename Func>
  ScheduleRegistrar& Register(const std::string& name, Func&& func);

  template <typename Cmpt, typename Func>
  ScheduleRegistrar& Register(const std::string& name, Func Cmpt::* func);

  ScheduleRegistrar& Order(std::string_view first, const std::string& second);
  template <typename CmptFirst, typename CmptSecond>
  ScheduleRegistrar& Order();
  template <typename Cmpt>
  ScheduleRegistrar& Before(std::string_view name);
  template <typename Cmpt>
  ScheduleRegistrar& After(const std::string& name);

  // TODO: regist not parallel

 private:
  friend class World;

  ScheduleRegistrar(ArchetypeMngr* mngr) noexcept : mngr{mngr} {}

  ArchetypeMngr* mngr;
  Schedule schedule;

  friend class CmptSysMngr;

  template <typename Cmpt>
  ScheduleRegistrar& Register();
};
}  // namespace My

#include "detail/ScheduleRegistrar.inl"
