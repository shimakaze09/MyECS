//
// Created by Admin on 26/12/2024.
//

#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "../config.h"

namespace My::MyECS {
using byte = uint8_t;
static_assert(sizeof(byte) == 1);

struct alignas(CHUNK_ALIGNMENT) Chunk {
  static constexpr std::size_t size = CHUNK_SIZE;

  struct Layout {
    std::size_t capacity;
    std::vector<std::size_t> offsets;
  };

  static Layout GenLayout(const std::vector<std::size_t>& alignments,
                          const std::vector<std::size_t>& sizes) noexcept;

  byte* Data() noexcept { return buffer.data(); }

 private:
  std::array<byte, size> buffer;
};

static_assert(sizeof(Chunk) == Chunk::size);
}  // namespace My::MyECS
