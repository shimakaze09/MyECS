//
// Created by Admin on 26/12/2024.
//

#pragma once

#include <MyECS/core/ArchetypeMngr.h>

using namespace My;

void ArchetypeMngr::Release(EntityData* e) {
  auto archetype = e->archetype();
  auto idx = e->idx();
  entityPool.recycle(e);

  auto [movedEntityIdx, pairs] = archetype->Erase(idx);

  if (movedEntityIdx != static_cast<size_t>(-1)) {
    auto target = d2p.find({archetype, movedEntityIdx});
    EntityData* movedEntity = target->second;
    for (auto p : pairs)
      movedEntity->MoveCmpt(p.first, p.second);
    movedEntity->idx() = idx;
    d2p[*e] = movedEntity;
    d2p.erase(target);
  }

  if (archetype->Size() == 0 && archetype->CmptNum() != 0) {
    ids.erase(archetype->id);
    id2a.erase(archetype->id);
    delete archetype;
  }

  archetype = nullptr;
  idx = static_cast<size_t>(-1);
}