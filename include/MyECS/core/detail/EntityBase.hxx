#ifndef ENTITY_DATA_HXX
#define ENTITY_DATA_HXX

#include <functional>
#include <map>

namespace My {
class Archetype;

struct EntityBase {
  Archetype* archetype{nullptr};
  size_t idx{static_cast<size_t>(-1)};

  ~EntityBase() {
    for (auto p : m_releases)
      p.second(p.first);
    m_releases.clear();

    archetype = nullptr;
    idx = static_cast<size_t>(-1);
  }

  bool operator<(const EntityBase& e) const noexcept {
    return archetype < e.archetype || (archetype == e.archetype && idx < e.idx);
  }

 private:
  friend class ArchetypeManager;
  friend class Archetype;

  template <typename Cmpt>
  void RegisterCmptRelease(Cmpt* cmpt) {
    m_releases[cmpt] = [](void* c) {
      static_cast<Cmpt*>(c)->~Cmpt();
    };
  }

  void MoveCmpt(void* src, void* dst) {
    auto target = m_releases.find(src);
    assert(target != m_releases.end());
    auto release = target->second;
    m_releases.erase(target);  // erase before change releases
    m_releases[dst] = release;
  }

  void ReleaseCmpt(void* cmpt) {
    auto target = m_releases.find(cmpt);
    assert(target != m_releases.end());
    target->second(cmpt);
    m_releases.erase(cmpt);
  }

 private:
  std::map<void*, std::function<void(void*)>> m_releases;
};
}  // namespace My

#endif  // ENTITY_DATA_HXX