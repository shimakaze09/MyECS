//
// Created by Admin on 27/12/2024.
//

#pragma once

namespace My {
template <typename... Cmpts>
static constexpr Chunk::Info<sizeof...(Cmpts)> Chunk::StaticInfo() noexcept {
  static_assert(sizeof...(Cmpts) > 0);
  constexpr size_t capacity = size / (sizeof(Cmpts) + ...);
  constexpr size_t N = sizeof...(Cmpts);
  constexpr std::array<size_t, N> sizes{sizeof(Cmpts)...};
  std::array<size_t, N> offsets{0};
  for (size_t i = 1; i < N; i++)
    offsets[i] = offsets[i - 1] + capacity * sizes[i - 1];
  return {capacity, sizes, offsets};
}
}  // namespace My
