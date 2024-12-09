#ifndef ENTITY_HXX
#define ENTITY_HXX

#include "ArchetypeManager.hxx"

namespace My {
class World;

class Entity final : private EntityBase {
 public:
  template <typename Cmpt>
  inline Cmpt* Get() {
    assert(IsAlive());
    return archetype->At<Cmpt>(idx);
  }

  template <typename... Cmpts>
  inline std::tuple<Cmpts*...> Attach() {
    static_assert(sizeof...(Cmpts) > 0);
    static_assert(IsSet_v<TypeList<Cmpts...>>, "Components must be different");
    static_assert(((std::is_constructible_v<Cmpts> ||
                    std::is_constructible_v<Cmpts, Entity*>) &&
                   ...));
    assert(IsAlive());
    return archetype->m_manager->EntityAttach<Cmpts...>(this);
  }

  template <typename Cmpt>
  inline Cmpt* GetOrAttach() {
    assert(IsAlive());
    Cmpt* cmpt = archetype->At<Cmpt>(idx);
    if (!cmpt)
      std::tie(cmpt) = Attach<Cmpt>();
    return cmpt;
  }

  template <typename... Cmpts>
  inline void Detach() {
    static_assert(sizeof...(Cmpts) > 0);
    static_assert(IsSet_v<TypeList<Cmpts...>>, "Components must be different");
    assert(IsAlive());
    return archetype->m_manager->EntityDetach<Cmpts...>(this);
  }

  inline bool IsAlive() const noexcept { return archetype != nullptr; }

  void Release() noexcept {
    assert(IsAlive());
    archetype->m_manager->Release(this);
  }

 private:
  friend class World;
};

static_assert(sizeof(Entity) == sizeof(EntityBase) &&
              std::is_base_of_v<EntityBase, Entity>);
}  // namespace My

#endif  // ENTITY_HXX