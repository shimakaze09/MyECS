//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <array>

namespace My {
static constexpr size_t size_t_invalid = static_cast<size_t>(-1);

inline constexpr size_t hash_combine(size_t x, size_t y) noexcept;
template <size_t N>
inline constexpr size_t hash_combine(const std::array<size_t, N>& arr) noexcept;
template <typename Container>
inline size_t hash_combine(const Container& container) noexcept;
}  // namespace My

#include "Util.inl"
