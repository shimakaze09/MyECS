//
// Created by Admin on 12/03/2025.
//

#include <MyECS/SystemMngr.h>

#include <cassert>

using namespace My::MyECS;

SystemMngr::SystemMngr(const SystemMngr& mngr, World* w)
    : systemTraits{mngr.systemTraits},
      aliveSystemIDs{mngr.aliveSystemIDs},
      activeSystemIDs{mngr.activeSystemIDs},
      w{w} {}

SystemMngr::SystemMngr(SystemMngr&& mngr, World* w) noexcept
    : systemTraits{std::move(mngr.systemTraits)},
      aliveSystemIDs{mngr.aliveSystemIDs},
      activeSystemIDs{mngr.activeSystemIDs},
      w{w} {}

SystemMngr::~SystemMngr() {
  assert(activeSystemIDs.empty());
  assert(aliveSystemIDs.empty());
}

void SystemMngr::Clear() {
  for (auto ID : activeSystemIDs)
    systemTraits.Deactivate(ID, w);
  for (auto ID : aliveSystemIDs)
    systemTraits.Destroy(ID, w);
  activeSystemIDs.clear();
  aliveSystemIDs.clear();
}

void SystemMngr::Create(std::size_t ID) {
  if (IsAlive(ID))
    return;
  systemTraits.Create(ID, w);
  aliveSystemIDs.insert(ID);
}

void SystemMngr::Activate(std::size_t ID) {
  Create(ID);
  if (IsActive(ID))
    return;
  systemTraits.Activate(ID, w);
  activeSystemIDs.insert(ID);
}

void SystemMngr::Update(std::size_t ID, Schedule& schedule) const {
  assert(IsActive(ID));
  systemTraits.Update(ID, schedule);
}

void SystemMngr::Deactivate(std::size_t ID) {
  if (!IsAlive(ID) || !IsActive(ID))
    return;
  systemTraits.Deactivate(ID, w);
  activeSystemIDs.erase(ID);
}

void SystemMngr::Destroy(std::size_t ID) {
  if (!IsAlive(ID))
    return;
  Deactivate(ID);
  systemTraits.Destroy(ID, w);
  aliveSystemIDs.erase(ID);
}

bool SystemMngr::IsAlive(std::size_t ID) const {
  return aliveSystemIDs.find(ID) != aliveSystemIDs.end();
}

bool SystemMngr::IsActive(std::size_t ID) const {
  return activeSystemIDs.find(ID) != activeSystemIDs.end();
}
