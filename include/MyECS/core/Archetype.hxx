#ifndef ARCHETYPE_HXX
#define ARCHETYPE_HXX

#include <MyTemplate/TypeID.hxx>

#include <cassert>
#include <map>
#include <vector>

namespace My::detail {
class Archetype {
 private:
  using byte = std::uint8_t;

 public:
  template <typename... Cmpts>
  void Init() {
    m_map.clear();
    m_size.clear();
    m_n = 0;
    if constexpr (sizeof...(Cmpts) == 0) {
      m_map[My::TypeID<void>] = std::vector<byte>{};
      m_size[My::TypeID<void>] = sizeof(byte);
    } else {
      ((m_map[My::TypeID<Cmpts>] = std::vector<byte>{}), ...);
      ((m_size[My::TypeID<Cmpts>] = sizeof(Cmpts)), ...);
    }
  }

  template <typename... Cmpts>
  size_t CreateEntity() {
    if constexpr (sizeof...(Cmpts) == 0)
      assert(m_map.size() == 1 && m_map.find(My::TypeID<void>) != m_map.end());
    else
      assert(!(sizeof...(Cmpts) != m_map.size()) ||
             !((m_map.find(My::TypeID<Cmpts>) != m_map.end()) && ...));

    (push_back<Cmpts>(), ...);

    return m_n++;
  }

  size_t CreateEntity() {
    assert(m_size.size() > 0);
    for (auto& p : m_size) {
      auto& arr = m_map[p.first];
      for (size_t i = 0; i < p.second; i++)
        arr.emplace_back();
    }
    return m_n++;
  }

  template <typename Cmpt, typename... Args>
  void Init(size_t ID, Args&&... args) {
    void* addr = &Get<Cmpt>(ID);
    new (addr) Cmpt{std::forward<Args>(args)...};
  }

  template <typename Cmpt>
  Cmpt& Get(size_t ID) {
    auto target = m_map.find(My::TypeID<Cmpt>);
    assert(target != m_map.end() &&
           target->second.size() >= sizeof(Cmpt) * (ID + 1));
    return reinterpret_cast<Cmpt&>(target->second[sizeof(Cmpt) * ID]);
  }

  template <typename Cmpt>
  Cmpt* Get() const noexcept {
    auto target = m_map.find(My::TypeID<Cmpt>);
    assert(target != m_map.end());
    return reinterpret_cast<Cmpt*>(const_cast<byte*>(target->second.data()));
  }

  template <typename... Cmpts>
  bool IsContain() const {
    return ((m_map.find(My::TypeID<Cmpts>) != m_map.end()) && ...);
  }

  size_t Delete(size_t ID) {
    assert(m_n > ID);
    if (ID != m_n - 1) {
      for (auto& p : m_map) {
        size_t s = m_size[p.first];
        auto& arr = p.second;
        auto dst = &arr[ID * s];
        auto src = &arr[(m_n - 1) * s];
        memcpy(dst, src, s);
        for (size_t i = 0; i < s; i++)
          arr.pop_back();
      }
    }
    return --m_n;
  }

  inline size_t Size() const noexcept { return m_n; }

 private:
  template <typename Cmpt>
  void push_back() {
    auto target = m_map.find(My::TypeID<Cmpt>);
    for (size_t i = 0; i < sizeof(Cmpt); i++)
      target->second.emplace_back();
  }

  size_t m_n{0};
  std::map<size_t, size_t> m_size;
  std::map<size_t, std::vector<byte>> m_map;
};
}  // namespace My::detail

#endif  // ARCHETYPE_HXX