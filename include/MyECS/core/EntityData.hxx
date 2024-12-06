#ifndef ENTITY_DATA_HXX
#define ENTITY_DATA_HXX

#include <tuple>

namespace My {
class Archetype;

struct EntityData : std::tuple<Archetype*, size_t> {
  EntityData(Archetype* archetype, size_t idx)
      : std::tuple<Archetype*, size_t>{archetype, idx} {}

  inline Archetype*& archetype() noexcept { return std::get<0>(*this); }

  inline const Archetype* archetype() const noexcept {
    return std::get<0>(*this);
  }

  inline size_t& idx() noexcept { return std::get<1>(*this); }

  inline size_t idx() const noexcept { return std::get<1>(*this); }
};
}  // namespace My

#endif  // ENTITY_DATA_HXX