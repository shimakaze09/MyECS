#pragma once

#include <compare>
#include <type_traits>

#include "details/Util.hpp"

namespace My::MyECS {
// index + version
struct Entity {
  std::size_t index;
  std::size_t version;

  static constexpr Entity Invalid() noexcept {
    return {.index = static_cast<std::size_t>(-1),
            .version = static_cast<std::size_t>(-1)};
  }
  constexpr bool Valid() const noexcept {
    return index != static_cast<std::size_t>(-1);
  }
  constexpr auto operator<=>(const Entity&) const = default;
};
static_assert(std::is_trivial_v<Entity>);
}  // namespace My::MyECS
