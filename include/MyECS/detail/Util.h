//
// Created by Admin on 4/03/2025.
//

#pragma once

#include <array>
#include <string_view>

namespace My::MyECS {
constexpr size_t hash_combine(size_t x, size_t y) noexcept;
template <size_t N>
constexpr size_t hash_combine(const std::array<size_t, N>& arr) noexcept;
template <typename Container>
size_t hash_combine(const Container& container) noexcept;

// fnv1a
constexpr size_t hash_string(std::string_view str) noexcept;
}  // namespace My::MyECS

#include "Util.inl"
