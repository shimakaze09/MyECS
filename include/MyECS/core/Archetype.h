//
// Created by Admin on 26/12/2024.
//

#pragma once

#include <MyTemplate/TypeID.h>

#include <cassert>
#include <map>
#include <vector>

namespace My::detail {
class ArcheType {
 private:
  using byte = std::uint8_t;

 public:
  template <typename... Cmpts>
  void Init() {
    map.clear();
    size.clear();
    n = 0;
    if constexpr (sizeof...(Cmpts) == 0) {
      map[My::TypeID<void>] = std::vector<byte>{};
      size[My::TypeID<void>] = sizeof(byte);
    } else {
      ((map[My::TypeID<Cmpts>] = std::vector<byte>{}), ...);
      ((size[My::TypeID<Cmpts>] = sizeof(Cmpts)), ...);
    }
  }

  template <typename... Cmpts>
  size_t CreateEntity() {
    if constexpr (sizeof...(Cmpts) == 0)
      assert(map.size() == 1 && map.find(My::TypeID<void>) != map.end());
    else
      assert(!(sizeof...(Cmpts) != map.size()) ||
             !((map.find(My::TypeID<Cmpts>) != map.end()) && ...));

    (push_back<Cmpts>(), ...);

    return n++;
  }

  size_t CreateEntity() {
    assert(size.size() > 0);
    for (auto& p : size) {
      auto& arr = map[p.first];
      for (size_t i = 0; i < p.second; i++)
        arr.emplace_back();
    }
    return n++;
  }

  template <typename Cmpt, typename... Args>
  void Init(size_t ID, Args&&... args) {
    void* addr = &Get<Cmpt>(ID);
    new (addr) Cmpt{std::forward<Args>(args)...};
  }

  template <typename Cmpt>
  Cmpt& Get(size_t ID) {
    auto target = map.find(My::TypeID<Cmpt>);
    assert(target != map.end() &&
           target->second.size() >= sizeof(Cmpt) * (ID + 1));
    return reinterpret_cast<Cmpt&>(target->second[sizeof(Cmpt) * ID]);
  }

  template <typename Cmpt>
  Cmpt* Get() const noexcept {
    auto target = map.find(My::TypeID<Cmpt>);
    assert(target != map.end());
    return reinterpret_cast<Cmpt*>(const_cast<byte*>(target->second.data()));
  }

  template <typename... Cmpts>
  bool IsContain() const {
    return ((map.find(My::TypeID<Cmpts>) != map.end()) && ...);
  }

  size_t Delete(size_t ID) {
    assert(n > ID);
    if (ID != n - 1) {
      for (auto& p : map) {
        size_t s = size[p.first];
        auto& arr = p.second;
        auto dst = &arr[ID * s];
        auto src = &arr[(n - 1) * s];
        memcpy(dst, src, s);
        for (size_t i = 0; i < s; i++)
          arr.pop_back();
      }
    }
    return --n;
  }

  inline size_t Size() const noexcept { return n; }

 private:
  template <typename Cmpt>
  void push_back() {
    auto target = map.find(My::TypeID<Cmpt>);
    for (size_t i = 0; i < sizeof(Cmpt); i++)
      target->second.emplace_back();
  }

  size_t n{0};
  std::map<size_t, size_t> size;
  std::map<size_t, std::vector<byte>> map;
};
}  // namespace My::detail
