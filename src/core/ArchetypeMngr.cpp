//
// Created by Admin on 26/12/2024.
//

#include <MyECS/detail/ArchetypeMngr.h>

using namespace My;
using namespace std;

ArchetypeMngr::~ArchetypeMngr() {
  for (auto p : id2a)
    delete p.second;
}

Archetype* ArchetypeMngr::GetArchetypeOf(const CmptIDSet& archetypeID) const {
  auto target = id2a.find(archetypeID);
  assert(target != id2a.end());
  return target->second;
}

void ArchetypeMngr::Release(EntityBase* e) {
  auto archetype = e->archetype;
  auto idx = e->idx;
  entityPool.Recycle(e);

  auto movedEntityIdx = archetype->Erase(idx);

  if (movedEntityIdx != static_cast<size_t>(-1)) {
    auto target = ai2e.find({archetype, movedEntityIdx});
    EntityBase* movedEntity = target->second;
    ai2e.erase(target);
    movedEntity->idx = idx;
    ai2e[{archetype, idx}] = movedEntity;
  }

  if (archetype->Size() == 0 && archetype->CmptNum() != 0) {
    ids.erase(archetype->id);
    id2a.erase(archetype->id);
    delete archetype;
  }

  archetype = nullptr;
  idx = static_cast<size_t>(-1);
}

void ArchetypeMngr::AddCommand(const std::function<void()>& command) {
  lock_guard<mutex> guard(commandBufferMutex);
  commandBuffer.push_back(command);
}

void ArchetypeMngr::RunCommands() {
  lock_guard<mutex> guard(commandBufferMutex);
  for (const auto& command : commandBuffer)
    command();
  commandBuffer.clear();
}
