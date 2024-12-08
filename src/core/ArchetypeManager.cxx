#include "MyECS/ArchetypeManager.hxx"

using namespace My;

void ArchetypeManager::Release(EntityData* e) {
  Archetype* archetype = e->archetype();
  size_t movedEntityIdx = archetype->Erase(e->idx());

  auto target = m_dataToPointer.find({archetype, movedEntityIdx});
  if (movedEntityIdx != static_cast<size_t>(-1)) {
    EntityData* movedEntity = target->second;
    movedEntity->idx() = e->idx();
    m_dataToPointer[*e] = movedEntity;
  }
  m_dataToPointer.erase(target);

  if (archetype->Size() == 0 && archetype->CmptNum() != 0) {
    m_ids.erase(archetype->id);
    m_idToArchetype.erase(archetype->id);
    delete archetype;
  }

  e->archetype() = nullptr;
  e->idx() = static_cast<size_t>(-1);

  m_entityPool.Recycle(e);
}