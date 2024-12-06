#ifndef ARCHETYPE_MANAGER_HXX
#define ARCHETYPE_MANAGER_HXX

#include "Archetype.hxx"

#include <MyTemplate/TypeList.hxx>

namespace My {
class World;
}

namespace My::detail {
class ArchetypeManager {
 public:
  ArchetypeManager(World* world) : m_world(world) {}

  inline World* World() const noexcept { return m_world; }

  template <typename... Cmpts>
  static constexpr size_t HashOf() noexcept {
    return My::TypeID<QuickSort_t<TypeList<Cmpts...>, My::TypeID_Less>>;
  }

  inline Archetype& GetArchetypeOf(size_t ArcheTypeHash) {
    auto target = m_map.find(ArcheTypeHash);
    assert(target != m_map.end());
    return target->second;
  }

  template <typename... Cmpts>
  inline Archetype& GetArchetypeOf() {
    constexpr size_t ArcheTypeHash =
        My::TypeID<QuickSort_t<TypeList<Cmpts...>, My::TypeID_Less>>;
    auto target = m_map.find(ArcheTypeHash);
    if (target == m_map.end()) {
      Archetype& c = m_map[ArcheTypeHash];
      c.Init<Cmpts...>();
      return c;
    } else
      return target->second;
  }

  template <typename... Cmpts>
  const std::vector<Archetype*> GetArchetypeWith() {
    std::vector<Archetype*> rst;
    if constexpr (sizeof...(Cmpts) == 0) {
      for (auto& p : m_map)
        rst.push_back(&p.second);
      return rst;
    } else {
      for (auto& p : m_map) {
        if (p.second.IsContain<Cmpts...>())
          rst.push_back(&p.second);
      }
      return rst;
    }
  }

 private:
  My::World* m_world;
  std::map<size_t, Archetype> m_map;
};
}  // namespace My::detail

#endif  // ARCHETYPE_MANAGER_HXX