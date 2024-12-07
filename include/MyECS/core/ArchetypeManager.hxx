#ifndef ARCHETYPE_MANAGER_HXX
#define ARCHETYPE_MANAGER_HXX

#include "Archetype.hxx"
#include "EntityData.hxx"
#include "Pool.hxx"

#include <MyTemplate/TypeList.hxx>

namespace My {
class World;

class ArchetypeManager {
 public:
  ArchetypeManager(World* world) : m_world(world) {}

  ~ArchetypeManager() {
    for (auto p : m_idToArchetype)
      delete p.second;
  }

  inline World* World() const noexcept { return m_world; }

  inline Archetype* GetArchetypeOf(const Archetype::ID& archetypeID) {
    auto target = m_idToArchetype.find(archetypeID);
    assert(target != m_idToArchetype.end());
    return target->second;
  }

  template <typename... Cmpts>
  inline Archetype* GetOrCreateArchetypeOf() {
    auto id = Archetype::ID(TypeList<Cmpts...>{});
    auto target = m_idToArchetype.find(id);
    if (target == m_idToArchetype.end()) {
      auto archetype = new Archetype(this, TypeList<Cmpts...>{});
      m_idToArchetype[id] = archetype;
      m_ids.insert(archetype->GetID());
      return archetype;
    } else
      return target->second;
  }

  template <typename... Cmpts>
  const std::vector<Archetype*> LocateArchetypeWith() {
    std::vector<Archetype*> rst;
    for (auto& p : m_idToArchetype) {
      if (p.second->IsContain<Cmpts...>())
        rst.push_back(p.second);
    }
    return rst;
  }

  template <typename... Cmpts>
  EntityData* CreateEntity() {
    Archetype* archetype = GetOrCreateArchetypeOf<Cmpts...>();
    size_t idx = archetype->CreateEntity<Cmpts...>();
    auto entity = m_entityPool.Request();
    entity->archetype() = archetype;
    entity->idx() = idx;
    m_dataToPointer[*entity] = entity;
    return entity;
  }

  template <typename Cmpt, typename... Args>
  Cmpt* EntityAdd(EntityData* e, Args&&... args) {
    Archetype* srcArchetype = e->archetype();
    size_t srcIdx = e->idx();

    auto& srcID = srcArchetype->GetID();
    auto dstID = srcID;
    dstID.Add<Cmpt>();

    Archetype* dstArchetype;
    auto target = m_idToArchetype.find(dstID);
    if (target == m_idToArchetype.end()) {
      dstArchetype = new Archetype(srcArchetype, IType<Cmpt>{});
      assert(dstID == dstArchetype->GetID());
      m_idToArchetype[dstID] = dstArchetype;
      m_ids.insert(dstID);
    } else
      dstArchetype = target->second;

    size_t dstIdx = dstArchetype->CreateEntity();
    Cmpt* cmpt = dstArchetype->Init<Cmpt>(dstIdx, std::forward<Args>(args)...);
    for (auto cmptHash : srcID) {
      auto srcCmpt = srcArchetype->At(cmptHash, srcIdx);
      auto dstCmpt = dstArchetype->At(cmptHash, dstIdx);
      size_t size = std::get<1>(srcCmpt);
      assert(size == std::get<1>(dstCmpt));
      memcpy(std::get<0>(dstCmpt), std::get<0>(srcCmpt), size);
    }

    size_t srcMovedIdx = srcArchetype->Erase(srcIdx);
    if (srcMovedIdx != static_cast<size_t>(-1)) {
      auto srcMovedEntityTarget = m_dataToPointer.find({srcArchetype, srcMovedIdx});
      auto srcMovedEntity = srcMovedEntityTarget->second;
      m_dataToPointer.erase(srcMovedEntityTarget);
      m_dataToPointer[{srcArchetype, srcIdx}] = srcMovedEntity;
      srcMovedEntity->idx() = srcMovedIdx;
    }

    m_dataToPointer[{dstArchetype, dstIdx}] = e;

    e->archetype() = dstArchetype;
    e->idx() = dstIdx;

    return cmpt;
  }

  void Release(EntityData* e) {
    size_t movedEntityIdx = e->archetype()->Erase(e->idx());

    auto target = m_dataToPointer.find({e->archetype(), movedEntityIdx});
    EntityData* movedEntity = target->second;
    m_dataToPointer.erase(target);

    movedEntity->idx() = e->idx();
    m_dataToPointer[*e] = movedEntity;

    m_entityPool.Recycle(e);
  }

 private:
  Pool<EntityData> m_entityPool;
  std::map<EntityData, EntityData*> m_dataToPointer;
  std::set<Archetype::ID> m_ids;
  My::World* m_world;
  std::map<Archetype::ID, Archetype*> m_idToArchetype;  // id to archetype
};
}  // namespace My

#endif  // ARCHETYPE_MANAGER_HXX