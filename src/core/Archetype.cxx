#include "MyECS/core/Archetype.hxx"

using namespace My;
using namespace std;

Pool<Chunk> Archetype::m_chunkPool;

bool Archetype::ID::operator<(const ID& id) const noexcept {
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

bool Archetype::ID::operator==(const ID& id) const noexcept {
  if (size() != id.size())
    return false;
  for (auto l = begin(), r = id.begin(); l != end(); ++l, ++r) {
    if (*l != *r)
      return false;
  }
  return true;
}

pair<void*, size_t> Archetype::At(size_t cmptHash, size_t idx) {
  auto target = m_hashToSO.find(cmptHash);
  if (target == m_hashToSO.end())
    return {nullptr, static_cast<size_t>(-1)};

  size_t size = target->second.first;
  size_t offset = target->second.second;
  size_t idxInChunk = idx % m_chunkCapacity;
  byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
  return {buffer + offset + size * idxInChunk, size};
}

pair<size_t, vector<pair<void*, void*>>> Archetype::Erase(size_t idx) {
  assert(idx < m_num);
  pair<size_t, vector<pair<void*, void*>>> rst;

  if (idx != m_num - 1) {
    size_t idxInChunk = idx % m_chunkCapacity;
    byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
    for (auto p : m_hashToSO) {
      size_t size = p.second.first;
      size_t offset = p.second.second;
      byte* dst = buffer + offset + size * idxInChunk;
      byte* src = buffer + offset + (m_num - 1) * idxInChunk;
      rst.second.emplace_back(dst, src);
      memcpy(dst, src, size);
    }
    rst.first = m_num - 1;
  } else
    rst.first = static_cast<size_t>(-1);

  m_num--;

  if (m_chunks.size() * m_chunkCapacity - m_num >= m_chunkCapacity) {
    Chunk* back = m_chunks.back();
    m_chunkPool.Recycle(back);
  }

  return rst;
}