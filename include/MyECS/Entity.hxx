#ifndef ENTITY_HXX
#define ENTITY_HXX

#include "ArchetypeManager.hxx"

namespace My {
class World;

class Entity : private EntityData {
 public:
  template <typename Cmpt, typename... Args>
  inline void Init(Args... args) {
    assert(IsAlive());
    archetype->Init<Cmpt>(idx, std::forward<Args>(args)...);
  }

  template <typename Cmpt>
  inline Cmpt* Get() {
    assert(IsAlive());
    return archetype()->At<Cmpt>(idx());
  }

  template <typename Cmpt, typename... Args>
  inline Cmpt* Attach(Args&&... args) {
    assert(IsAlive());
    return archetype()->m_mgr->EntityAttach<Cmpt>(this,
                                                  std::forward<Args>(args)...);
  }

  template <typename Cmpt>
  inline void Detach() {
    assert(IsAlive());
    return archetype()->m_mgr->EntityDetach<Cmpt>(this);
  }

  inline bool IsAlive() const noexcept { return archetype() != nullptr; }

 private:
  friend class World;
};

static_assert(sizeof(Entity) == sizeof(EntityData) &&
              std::is_base_of_v<EntityData, Entity>);
}  // namespace My

#endif  // ENTITY_HXX