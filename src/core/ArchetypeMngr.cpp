//
// Created by Admin on 26/12/2024.
//

#pragma once

#include <MyECS/ArchetypeMngr.h>

using namespace My;

void ArchetypeMngr::Release(EntityData* e) {
  size_t movedEntityIdx = e->archetype()->Erase(e->idx());

  auto target = d2p.find({e->archetype(), movedEntityIdx});
  EntityData* movedEntity = target->second;
  d2p.erase(target);

  movedEntity->idx() = e->idx();
  d2p[*e] = movedEntity;

  entityPool.recycle(e);
}
