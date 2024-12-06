#ifndef ENTITY_HXX
#define ENTITY_HXX

#include "Archetype.hxx"
#include "Component.hxx"

namespace My {
class World;

class Entity {
 public:
  template <typename Cmpt, typename... Args>
  inline void Init(Args... args) {
    m_archetype->Init<Cmpt>(m_id, std::forward<Args>(args)...);
  }

  template <typename Cmpt>
  inline Cmpt& Get() {
    return m_archetype->Get<Cmpt>(m_id);
  }

  inline bool IsAlive() const noexcept { return m_isAlive; }

 private:
  friend class World;
  bool m_isAlive{false};
  detail::Archetype* m_archetype;
  size_t m_id;
};
}  // namespace My

#endif  // ENTITY_HXX