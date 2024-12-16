#ifndef ENTITYBASE_HXX
#define ENTITYBASE_HXX

#include <functional>
#include <map>

namespace My {
class Archetype;

struct EntityBase {
  Archetype* archetype{nullptr};
  size_t idx{static_cast<size_t>(-1)};

  bool operator<(const EntityBase& e) const noexcept {
    return archetype < e.archetype || (archetype == e.archetype && idx < e.idx);
  }

 private:
  friend class ArchetypeManager;
  friend class Archetype;
};
}  // namespace My

#endif  //ENTITYBASE_HXX
