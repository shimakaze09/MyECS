//
// Created by Admin on 13/03/2025.
//

#pragma once

#include "Schedule.h"

namespace My::MyECS {
class World;

// stateless
class System {
 public:
  System(World* world, std::string name) noexcept : world{world}, name{name} {}

  World* GetWorld() const noexcept { return world; }

  const std::string& GetName() const noexcept { return name; }

  virtual void OnUpdate(Schedule&) = 0;

 private:
  World* world;
  std::string name;
};
}  // namespace My::MyECS
