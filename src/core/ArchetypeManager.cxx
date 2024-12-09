#include "MyECS/core/ArchetypeManager.hxx"

using namespace My;

void ArchetypeManager::Release(EntityData* e) {
  auto archetype = e->archetype();
  auto idx = e->idx();
  m_entityPool.Recycle(e);

  auto [movedEntityIdx, pairs] = archetype->Erase(idx);

  if (movedEntityIdx != static_cast<size_t>(-1)) {
    auto target = m_dataToPointer.find({archetype, movedEntityIdx});
    EntityData* movedEntity = target->second;
    for (auto p : pairs)
      movedEntity->MoveCmpt(p.first, p.second);
    movedEntity->idx() = idx;
    m_dataToPointer[*e] = movedEntity;
    m_dataToPointer.erase(target);
  }

  if (archetype->Size() == 0 && archetype->CmptNum() != 0) {
    m_ids.erase(archetype->id);
    m_idToArchetype.erase(archetype->id);
    delete archetype;
  }

  archetype = nullptr;
  idx = static_cast<size_t>(-1);
}