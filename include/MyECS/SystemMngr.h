//
// Created by Admin on 21/02/2025.
//

#pragma once

#include "Schedule.h"

#include <MyContainer/xSTL/xMap.h>

namespace My {
// System Manager
// System is a struct with specific function
// signature: static void OnUpdate(Schedule&)
class SystemMngr {
 public:
  void Register(std::string name, std::function<void(Schedule&)> onUpdate) {
    onUpdateMap.emplace(std::move(name), std::move(onUpdate));
  }

  bool IsRegister(std::string_view name) const {
    return onUpdateMap.find(name) != onUpdateMap.end();
  }

  void Deregister(const std::string& name) { onUpdateMap.erase(name); }

  template <typename... Systems>
  void Register();
  template <typename System>
  bool IsRegistered() const;
  template <typename... Systems>
  void Deregister() noexcept;

 private:
  template <typename System>
  void RegisterOne();
  template <typename System>
  void DeregisterOne();

  xMap<std::string, std::function<void(Schedule&)>> onUpdateMap;

  friend class World;
};
}  // namespace My

#include "detail/SystemMngr.inl"
