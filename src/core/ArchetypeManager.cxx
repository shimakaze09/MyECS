#include "MyECS/core/ArchetypeManager.hxx"

using namespace My;

void ArchetypeManager::Release(EntityData* e) {
  size_t movedEntityIdx = e->archetype()->Erase(e->idx());

  auto target = m_dataToPointer.find({e->archetype(), movedEntityIdx});
  EntityData* movedEntity = target->second;
  m_dataToPointer.erase(target);

  movedEntity->idx() = e->idx();
  m_dataToPointer[*e] = movedEntity;

  m_entityPool.Recycle(e);
}