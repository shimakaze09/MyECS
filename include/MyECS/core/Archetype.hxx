#ifndef ARCHETYPE_HXX
#define ARCHETYPE_HXX

#include "Chunk.hxx"

#include <MyTemplate/TypeID.hxx>

#include <cassert>
#include <map>
#include <set>
#include <tuple>
#include <vector>

namespace My {
class ArchetypeManager;

class Archetype {
 public:
  class ID : private std::set<size_t> {
   public:
    ID() = default;

    template <typename... Cmpts>
    ID(TypeList<Cmpts...>) noexcept {
      Add<Cmpts...>();
    }

    template <typename... Cmpts>
    void Add() noexcept {
      (insert(TypeID<Cmpts>), ...);
    }

    template <typename... Cmpts>
    bool IsContain() const noexcept {
      return ((find(TypeID<Cmpts>) != end()) && ...);
    }

    template <typename... Cmpts>
    bool Is() const noexcept {
      return sizeof...(Cmpts) == size() && IsContain<Cmpts...>();
    }

    using std::set<size_t>::begin;
    using std::set<size_t>::end;

    bool operator<(const ID& id) const noexcept {
      auto l = begin(), r = id.begin();
      while (l != end() && r != id.end()) {
        if (*l < *r)
          return true;
        if (*l > *r)
          return false;
        ++l;
        ++r;
      }
      return l == end() && r != id.end();
    }

    bool operator==(const ID& id) const noexcept {
      if (size() != id.size())
        return false;
      for (auto l = begin(), r = id.begin(); l != end(); ++l, ++r) {
        if (*l != *r)
          return false;
      }
      return true;
    }
  };

  // argument is for type deduction
  template <typename... Cmpts>
  Archetype(ArchetypeManager* mgr, TypeList<Cmpts...>) : m_mgr(mgr) {
    using CmptList = TypeList<Cmpts...>;

    constexpr size_t N = sizeof...(Cmpts);

    constexpr auto info = Chunk::StaticInfo<Cmpts...>();
    m_chunkCapacity = info.capacity;
    ((h2so[TypeID<Cmpts>] =
          std::make_tuple(info.sizes[Find_v<CmptList, Cmpts>],
                          info.offsets[Find_v<CmptList, Cmpts>])),
     ...);
    id.Add<Cmpts...>();
  }

  template <typename Cmpt>
  Archetype(Archetype* srcArchetype, IType<Cmpt>) {
    ArchetypeManager* mgr = srcArchetype->m_mgr;
    id = srcArchetype->id;
    id.Add<Cmpt>();
    std::map<size_t, size_t> s2h;  // size to hash
    for (auto h : id) {
      if (h == TypeID<Cmpt>)
        s2h[sizeof(Cmpt)] = h;
      else
        s2h[std::get<0>(srcArchetype->h2so[h])] = h;
    }
    std::vector<size_t> sizes;
    for (auto p : s2h)
      sizes.push_back(p.first);  // sorted
    auto co = Chunk::CO(sizes);
    m_chunkCapacity = std::get<0>(co);
    size_t i = 0;
    for (auto h : id) {
      h2so[h] = std::make_tuple(sizes[i], std::get<1>(co)[i]);
      i++;
    }
  }

  ~Archetype() {
    for (auto c : m_chunks)
      delete c;
  }

  template <typename... Cmpts>
  const std::tuple<std::vector<Cmpts*>...> Locate() {
    return {LocateOne<Cmpts>()...};
  }

  std::tuple<void*, size_t> At(size_t cmptHash, size_t idx) {
    auto target = h2so.find(cmptHash);
    assert(target != h2so.end());
    size_t size = std::get<0>(target->second);
    size_t offset = std::get<1>(target->second);
    size_t idxInChunk = idx % m_chunkCapacity;
    byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
    return {buffer + offset + size * idxInChunk, size};
  }

  template <typename Cmpt>
  Cmpt* At(size_t idx) {
    auto target = h2so.find(TypeID<Cmpt>);
    if (target == h2so.end())
      return nullptr;
    assert(sizeof(Cmpt) == std::get<0>(target->second));
    size_t offset = std::get<1>(target->second);
    size_t idxInChunk = idx % m_chunkCapacity;
    byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
    return reinterpret_cast<Cmpt*>(buffer + offset + sizeof(Cmpt) * idxInChunk);
  }

  // no init
  size_t CreateEntity() {
    if (m_num % m_chunkCapacity == 0)
      m_chunks.push_back(new Chunk);
    return m_num++;
  }

  // init all cmpts
  template <typename... Cmpts>
  size_t CreateEntity() {
    assert(id.Is<Cmpts...>());

    using CmptList = TypeList<Cmpts...>;
    size_t idx = CreateEntity();
    size_t idxInChunk = idx % m_chunkCapacity;
    byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
    std::array<std::tuple<size_t, size_t>, sizeof...(Cmpts)> soArr{
        h2so[TypeID<Cmpts>]...};
    (new (buffer + std::get<1>(soArr[Find_v<CmptList, Cmpts>]) +
          idxInChunk * std::get<0>(soArr[Find_v<CmptList, Cmpts>])) Cmpts(),
     ...);

    return idx;
  }

  // erase idx-th entity
  // if idx != num-1, back entity will put at idx, return num-1
  // else return static_cast<size_t>(-1)
  size_t Erase(size_t idx) {
    assert(idx < m_num);
    size_t movedIdx;
    if (idx != m_num - 1) {
      size_t idxInChunk = idx % m_chunkCapacity;
      byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
      for (auto p : h2so) {
        size_t size = std::get<0>(p.second);
        size_t offset = std::get<1>(p.second);
        byte* dst = buffer + offset + size * idxInChunk;
        byte* src = buffer + offset + (m_num - 1) * idxInChunk;
        memcpy(dst, src, size);
      }
      movedIdx = m_num - 1;
    } else
      movedIdx = static_cast<size_t>(-1);

    m_num--;

    return movedIdx;
  }

  inline size_t Size() const noexcept { return m_num; }

  inline size_t ChunkNum() const noexcept { return m_chunks.size(); }

  inline size_t ChunkCapacity() const noexcept { return m_chunkCapacity; }

  inline const ID& GetID() const noexcept { return id; }

  inline ArchetypeManager* GetArchetypeManager() const noexcept {
    return m_mgr;
  }

  template <typename... Cmpts>
  bool IsContain() const noexcept {
    return id.IsContain<Cmpts...>();
  }

  template <typename Cmpt, typename... Args>
  Cmpt* Init(size_t idx, Args&&... args) noexcept {
    Cmpt* cmpt = reinterpret_cast<Cmpt*>(At<Cmpt>(idx));
    new (cmpt) Cmpt(std::forward<Args>(args)...);
    return cmpt;
  }

 private:
  template <typename Cmpt>
  const std::vector<Cmpt*> LocateOne() {
    auto target = h2so.find(TypeID<Cmpt>);
    assert(target != h2so.end());
    const size_t offset = std::get<1>(target->second);
    std::vector<Cmpt*> rst;
    for (auto c : m_chunks)
      rst.push_back(reinterpret_cast<Cmpt*>(c->Data() + offset));
    return rst;
  }

 private:
  friend class Entity;

  size_t m_num{0};
  ArchetypeManager* m_mgr;
  ID id;
  std::map<size_t, std::tuple<size_t, size_t>> h2so;  // hash to {size, offset}
  size_t m_chunkCapacity;
  std::vector<Chunk*> m_chunks;
};
}  // namespace My

#endif  // ARCHETYPE_HXX