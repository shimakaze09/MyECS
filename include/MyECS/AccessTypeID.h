#pragma once

#include "CmptTag.h"

#include "details/Util.h"

#include <MyTemplate/Type.h>

#include <set>

namespace My::MyECS {
// TypeID with AccessMode
class AccessTypeID : public TypeID {
 public:
  using TypeID::TypeID;  // AccessMode::WRITE

  constexpr AccessTypeID(std::size_t ID, AccessMode mode) noexcept
      : TypeID{ID}, mode{mode} {}

  constexpr AccessTypeID(std::string_view type_name, AccessMode mode) noexcept
      : TypeID{type_name}, mode{mode} {}

  constexpr AccessTypeID(TypeID ID,
                         AccessMode mode = AccessMode::WRITE) noexcept
      : TypeID{ID}, mode{mode} {}

  template <std::size_t N>
  constexpr AccessTypeID(const char (&str)[N], AccessMode mode) noexcept
      : TypeID{str}, mode{mode} {}

  constexpr AccessMode GetAccessMode() const noexcept { return mode; }

 private:
  AccessMode mode{AccessMode::WRITE};
};

template <typename Cmpt>
static constexpr AccessTypeID AccessTypeID_of = {TypeID_of<RemoveTag_t<Cmpt>>,
                                                 AccessMode_of<Cmpt>};

using AccessTypeIDSet = std::set<AccessTypeID, std::less<>>;
}  // namespace My::MyECS

template <>
struct std::hash<My::MyECS::AccessTypeID> {
  constexpr std::size_t operator()(
      const My::MyECS::AccessTypeID& id) const noexcept {
    return id.GetValue();
  }
};
