#pragma once

#include <MyECS/config.h>

#include <array>
#include <cstdint>
#include <vector>

namespace My::MyECS {
using byte = uint8_t;
static_assert(sizeof(byte) == 1);

struct alignas(ChunkAlignment) Chunk {
  static constexpr std::size_t size = ChunkSize;

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
