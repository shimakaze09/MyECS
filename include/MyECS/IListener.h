//
// Created by Admin on 12/03/2025.
//

#pragma once

#include <string_view>

namespace My::MyECS {
class World;
class SystemMngr;
class EntityMngr;
class Entity;
class CmptPtr;

class IListener {
 public:
  virtual void EnterWorld(const World* w) = 0;
  virtual void ExistWorld(const World* w) = 0;

  virtual void EnterSystemMngr(const SystemMngr* sm) = 0;
  virtual void ExistSystemMngr(const SystemMngr* sm) = 0;

  virtual void EnterSystem(std::string_view s) = 0;
  virtual void ExistSystem(std::string_view s) = 0;

  virtual void EnterEntityMngr(const EntityMngr* em) = 0;
  virtual void ExistEntityMngr(const EntityMngr* em) = 0;

  virtual void EnterEntity(const Entity* e) = 0;
  virtual void ExistEntity(const Entity* e) = 0;

  virtual void EnterCmptPtr(const CmptPtr* cmpt) = 0;
  virtual void ExistCmptPtr(const CmptPtr* cmpt) = 0;
};
}  // namespace My::MyECS
