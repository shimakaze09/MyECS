#ifndef WORLD_INL
#define WORLD_INL

namespace My {
template <typename... Cmpts>
Entity* World::CreateEntity() {
  return reinterpret_cast<Entity*>(m_manager->CreateEntity<Cmpts...>());
}
}  // namespace My

namespace My::detail::World_ {
template <typename... Cmpts>
struct Each<TypeList<Cmpts*...>> {
  static_assert(sizeof...(Cmpts) > 0);
  using CmptList = TypeList<Cmpts...>;

  template <typename Sys>
  static void Run(World* w, Sys&& s) {
    for (auto archetype : w->m_manager->GetArchetypeWith<Cmpts...>()) {
      auto cmptsVecTuple = archetype->Locate<Cmpts...>();
      size_t num = archetype->Size();
      size_t chunkNum = archetype->ChunkNum();
      size_t chunkCapacity = archetype->ChunkCapacity();
      for (size_t i = 0; i < chunkNum - 1; i++) {
        auto cmptsTuple = std::make_tuple(
            std::get<Find_v<CmptList, Cmpts>>(cmptsVecTuple)[i]...);
        for (size_t j = 0; j < chunkCapacity; j++)
          s((std::get<Find_v<CmptList, Cmpts>>(cmptsTuple) + j)...);
      }
      auto cmptsTuple = std::make_tuple(
          std::get<Find_v<CmptList, Cmpts>>(cmptsVecTuple)[chunkNum - 1]...);
      for (size_t j = 0; j < num - ((chunkNum - 1) * chunkCapacity); j++)
        s((std::get<Find_v<CmptList, Cmpts>>(cmptsTuple) + j)...);
    }
  }
};
}  // namespace My::detail::World_

#endif  // WORLD_INL
