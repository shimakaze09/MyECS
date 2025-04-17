#pragma once

#include <MyTemplate/Type.hpp>
#include <cstdint>
#include <span>

#include "Entity.hpp"
#include "config.hpp"

namespace My::MyECS {
class Archetype;

class alignas(ChunkAlignment) Chunk {
 public:
  bool Contains(TypeID) const noexcept;

  bool DidChange(TypeID cmptType, std::size_t version) const noexcept;
  bool DidOrderChange(std::size_t version) const noexcept;

  std::uint64_t GetComponentVersion(TypeID cmptType) const noexcept;
  std::uint64_t GetOrderVersion() const noexcept {
    return GetHead()->order_version;
  }

  std::size_t EntityNum() const noexcept { return GetHead()->num_entity; }
  std::size_t ComponentNum() const noexcept { return GetHead()->num_component; }

  // if not contains the component, return nullptr
  void* GetCmptArray(TypeID cmptType) const noexcept;

  // if not contains the component, return nullptr
  template <typename Cmpt>
  std::span<Cmpt> GetCmptArray() const noexcept {
    return {(Cmpt*)GetCmptArray(TypeID_of<Cmpt>), GetHead()->num_component};
  }

  std::span<Entity> GetEntityArray() const noexcept {
    return GetCmptArray<Entity>();
  }

  bool Full() const noexcept {
    return GetHead()->capacity == GetHead()->num_entity;
  }

  bool HasAnyChange(std::span<const TypeID> types,
                    std::uint64_t version) const noexcept;

 private:
  friend Archetype;

  struct Head {
    Archetype* archetype;
    std::uint64_t num_entity;
    std::uint64_t num_component;
    std::uint64_t capacity;
    std::uint64_t order_version;

    struct CmptInfo {
      TypeID ID;
      std::uint64_t offset;
      std::uint64_t version;
      friend bool operator<(const CmptInfo& lhs, const CmptInfo& rhs) noexcept {
        return lhs.ID < rhs.ID;
      }
      friend bool operator<(const CmptInfo& lhs, const TypeID& rhs) noexcept {
        return lhs.ID < rhs;
      }
      friend bool operator<(const TypeID& lhs, const CmptInfo& rhs) noexcept {
        return lhs < rhs.ID;
      }
    };  // 24 bytes
    static_assert(sizeof(CmptInfo) == 24);

    // sorted by ID
    std::span<CmptInfo> GetCmptInfos() noexcept {
      return {(CmptInfo*)(this + 1), num_component};
    }
    std::span<const CmptInfo> GetCmptInfos() const noexcept {
      return const_cast<Head*>(this)->GetCmptInfos();
    }

    void UpdateVersion(std::uint64_t version);
  };  // 40 bytes
  static_assert(sizeof(Head) == 40);

  Head* GetHead() noexcept { return reinterpret_cast<Head*>(data); }
  const Head* GetHead() const noexcept {
    return reinterpret_cast<const Head*>(data);
  }

  static_assert(ChunkSize > sizeof(Head));
  std::uint8_t data[ChunkSize];
};

class ChunkView {
 public:
  ChunkView(const Chunk* c = nullptr) : chunk{c} {}
  const Chunk* GetChunk() const noexcept { return chunk; }
  const Chunk* operator->() const noexcept { return chunk; }

 private:
  const Chunk* chunk;
};
}  // namespace My::MyECS
