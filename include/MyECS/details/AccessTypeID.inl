#pragma once

namespace std {
template <typename T>
struct hash;
}

template <>
struct std::hash<My::MyECS::AccessTypeID> {
  constexpr std::size_t operator()(
      const My::MyECS::AccessTypeID& id) const noexcept {
    return id.GetValue();
  }
};
