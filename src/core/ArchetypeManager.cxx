#include "MyECS/core/detail/ArchetypeManager.hxx"

using namespace My;

void ArchetypeManager::Release(EntityBase* e) {
  auto archetype = e->archetype;
  auto idx = e->idx;
  m_entityPool.Recycle(e);

  auto [movedEntityIdx, pairs] = archetype->Erase(idx);

  if (movedEntityIdx != static_cast<size_t>(-1)) {
    auto target = m_aiToEntity.find({archetype, movedEntityIdx});
    EntityBase* movedEntity = target->second;
    for (auto p : pairs)
      movedEntity->MoveCmpt(p.first, p.second);
    movedEntity->idx = idx;
    m_aiToEntity[{archetype, idx}] = movedEntity;
    m_aiToEntity.erase(target);
  }

  if (archetype->Size() == 0 && archetype->CmptNum() != 0) {
    m_ids.erase(archetype->m_id);
    m_idToArchetype.erase(archetype->m_id);
    delete archetype;
  }

  archetype = nullptr;
  idx = static_cast<size_t>(-1);
}