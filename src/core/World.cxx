#include "MyECS/core/World.hxx"

using namespace My;

void World::Delete(Entity* entity) {
  size_t movedId = entity->m_archetype->Delete(entity->m_id);

  auto target =
      m_atidToEntity.find(std::make_tuple(entity->m_archetype, movedId));
  auto movedEntity = target->second;
  m_atidToEntity.erase(target);
  m_atidToEntity[std::make_tuple(entity->m_archetype, entity->m_id)] =
      movedEntity;
  m_entityToAtid[movedEntity] =
      std::make_tuple(entity->m_archetype, entity->m_id);
  movedEntity->m_id = entity->m_id;

  m_entityToAtid.erase(entity);
  m_atidToEntity.erase(std::make_tuple(entity->m_archetype, entity->m_id));
  entity->m_isAlive = false;
  m_entities.Recycle(entity);
}