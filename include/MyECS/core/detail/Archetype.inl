#ifndef ARCHETYPE_INL
#define ARCHETYPE_INL

namespace My {
template <typename... Cmpts>
Archetype::Archetype(ArchetypeManager* mgr, TypeList<Cmpts...>) noexcept
    : m_manager(mgr), m_id(TypeList<Cmpts...>{}) {
  using CmptList = TypeList<Cmpts...>;

  constexpr size_t N = sizeof...(Cmpts);

  constexpr auto info = Chunk::StaticInfo<Cmpts...>();
  m_chunkCapacity = info.capacity;
  ((m_hashToSO[TypeID<Cmpts>] =
        std::make_pair(info.sizes[Find_v<CmptList, Cmpts>],
                       info.offsets[Find_v<CmptList, Cmpts>])),
   ...);
}

template <typename... Cmpts>
Archetype* Archetype::Add<Cmpts...>::From(Archetype* srcArchetype) noexcept {
  using CmptList = TypeList<Cmpts...>;
  assert(((!srcArchetype->m_id.IsContain<Cmpts>()) && ...));
  Archetype* rst = new Archetype;
  rst->m_manager = srcArchetype->m_manager;

  rst->m_id = srcArchetype->m_id;
  rst->m_id.Add<Cmpts...>();

  std::vector<size_t> h;
  std::vector<size_t> s;
  ((h.push_back(TypeID<Cmpts>), s.push_back(sizeof(Cmpts))), ...);
  for (auto p : srcArchetype->m_hashToSO) {
    h.push_back(p.first);
    s.push_back(std::get<0>(p.second));
  }
  auto co = Chunk::CO(s);
  rst->m_chunkCapacity = std::get<0>(co);
  ((rst->m_hashToSO[TypeID<Cmpts>] = std::make_pair(
        s[Find_v<CmptList, Cmpts>], std::get<1>(co)[Find_v<CmptList, Cmpts>])),
   ...);
  for (size_t i = sizeof...(Cmpts); i < rst->m_id.size(); i++)
    rst->m_hashToSO[h[i]] = std::make_pair(s[i], std::get<1>(co)[i]);
  return rst;
}

template <typename... Cmpts>
Archetype* Archetype::Remove<Cmpts...>::From(Archetype* srcArchetype) noexcept {
  using CmptList = TypeList<Cmpts...>;
  assert((srcArchetype->m_id.IsContain<Cmpts>() && ...));

  Archetype* rst = new Archetype;
  rst->m_manager = srcArchetype->m_manager;

  rst->m_id = srcArchetype->m_id;
  rst->m_id.Remove<Cmpts...>();

  std::vector<size_t> h;
  std::vector<size_t> s;
  for (auto p : rst->m_hashToSO) {
    if (rst->m_id.IsContain(p.first))
      continue;
    h.push_back(p.first);
    s.push_back(std::get<0>(p.second));
  }
  auto co = Chunk::CO(s);
  rst->m_chunkCapacity = std::get<0>(co);
  for (size_t i = 0; i < rst->m_id.size(); i++)
    rst->m_hashToSO[h[i]] = std::make_pair(s[i], std::get<1>(co)[i]);
  return rst;
}

template <typename Cmpt>
Cmpt* Archetype::At(size_t idx) {
  auto target = m_hashToSO.find(TypeID<Cmpt>);
  if (target == m_hashToSO.end())
    return nullptr;
  assert(sizeof(Cmpt) == target->second.first);
  size_t offset = target->second.second;
  size_t idxInChunk = idx % m_chunkCapacity;
  byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
  return reinterpret_cast<Cmpt*>(buffer + offset + sizeof(Cmpt) * idxInChunk);
}

template <typename... Cmpts>
const std::pair<size_t, std::tuple<Cmpts*...>> Archetype::CreateEntity(
    EntityBase* e) {
  assert(m_id.Is<Cmpts...>());

  using CmptList = TypeList<Cmpts...>;
  size_t idx = CreateEntity();
  size_t idxInChunk = idx % m_chunkCapacity;
  byte* buffer = m_chunks[idx / m_chunkCapacity]->Data();
  std::array<std::pair<size_t, size_t>, sizeof...(Cmpts)> soArr{
      m_hashToSO[TypeID<Cmpts>]...};
  std::tuple<Cmpts*...> cmpts = {
      New<Cmpts>(buffer + soArr[Find_v<CmptList, Cmpts>].second +
                     idxInChunk * soArr[Find_v<CmptList, Cmpts>].first,
                 e)...};

  return {idx, cmpts};
}

template <typename Cmpt>
const std::vector<Cmpt*> Archetype::LocateOne() {
  auto target = m_hashToSO.find(TypeID<Cmpt>);
  assert(target != m_hashToSO.end());
  assert(sizeof(Cmpt) == target->second.first);
  const size_t offset = target->second.second;
  std::vector<Cmpt*> rst;
  for (auto c : m_chunks)
    rst.push_back(reinterpret_cast<Cmpt*>(c->Data() + offset));
  return rst;
}

template <typename Cmpt>
Cmpt* Archetype::New(void* addr, EntityBase* e) {
  Cmpt* cmpt;
  if constexpr (std::is_constructible_v<Cmpt, Entity*>)
    cmpt = new (addr) Cmpt(reinterpret_cast<Entity*>(e));
  else
    cmpt = new (addr) Cmpt;
  e->RegisterCmptRelease(cmpt);
  return cmpt;
}

template <typename Cmpt>
Cmpt* Archetype::New(size_t idx, EntityBase* e) {
  return New<Cmpt>(At<Cmpt>(idx), e);
}
}  // namespace My

#endif  // ARCHETYPE_INL