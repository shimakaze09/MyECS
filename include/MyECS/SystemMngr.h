//
// Created by Admin on 21/02/2025.
//

#pragma once

#include "Schedule.h"
#include "System.h"

#include <MyContainer/xSTL/xMap.h>

#include <memory>

namespace My::MyECS {
class IListener;
class World;

// System Manager
// System is a struct with specific function
// signature: static void OnUpdate(Schedule&)
class SystemMngr {
 public:
  SystemMngr(World* world) : world{world} {}

  void Register(std::unique_ptr<System> system) {
    assert(system.get() != nullptr);
    systems.emplace(system->GetName(), std::move(system));
  }

  bool IsRegister(std::string_view name) const {
    return systems.find(name) != systems.end();
  }

  void Deregister(std::string_view name) { systems.erase(systems.find(name)); }

  // Systems must be derived form System and std::is_constructable_v<Systems, World*>
  template <typename... DerivedSystems>
  void Register();

  template <typename DerivedSystem>
  bool IsRegistered() const;

  template <typename... DerivedSystems>
  void Deregister() noexcept;

  void Accept(IListener*) const;

 private:
  template <typename DerivedSystem>
  void RegisterOne();
  template <typename DerivedSystem>
  void DeregisterOne();

  xMap<std::string, std::unique_ptr<System>> systems;
  World* world;

  friend class World;
};
}  // namespace My::MyECS

#include "detail/SystemMngr.inl"
