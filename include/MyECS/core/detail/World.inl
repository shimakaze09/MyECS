#ifndef WORLD_INL
#define WORLD_INL

namespace My::detail::World_ {
template <typename... Cmpts>
struct Each<TypeList<Cmpts*...>> {
  using CmptList = TypeList<Cmpts...>;

  template <typename Sys>
  static void Run(World* w, Sys&& s) {
    for (auto archetype : w->m_manager.GetArchetypeWith<Cmpts...>()) {
      std::array<void*, sizeof...(Cmpts)> arr = {archetype->Get<Cmpts>()...};
      for (size_t i = 0; i < archetype->Size(); ++i)
        s((reinterpret_cast<Cmpts*>(arr[Find_v<CmptList, Cmpts>]) + i)...);
    }
  }
};
}  // namespace My::detail::World_

namespace My {
template <typename... Cmpts>
Entity* World::CreateEntityWith() {
  detail::Archetype& c = m_manager.GetArchetypeOf<Cmpts...>();
  size_t id = c.CreateEntity<Cmpts...>();

  auto entity = m_entities.Request();
  entity->m_isAlive = true;
  entity->m_id = id;
  entity->m_archetype = &c;

  m_atidToEntity[std::make_tuple(&c, id)] = entity;
  m_entityToAtid[entity] = std::make_tuple(&c, id);

  return entity;
}
}  // namespace My

#endif  // WORLD_INL
