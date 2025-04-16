#pragma once

#include <MyTemplate/Type.h>
#include <small_vector.h>

#include <functional>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

namespace My::MyECS {
class RTDCmptTraits;

// run-time static component traits
class ArchetypeCmptTraits {
 public:
  bool IsTrivial(TypeID) const;
  std::size_t Sizeof(TypeID) const;
  std::size_t Alignof(TypeID) const;
  void CopyConstruct(TypeID, void* dst, void* src) const;
  void MoveConstruct(TypeID, void* dst, void* src) const;
  void MoveAssign(TypeID, void* dst, void* src) const;
  void Destruct(TypeID, void* cmpt) const;

  struct CmptTraits {
    TypeID ID;
    bool trivial;
    size_t size;
    size_t alignment;
    std::function<void(void*, void*)> copy_ctor;    // dst <- src
    std::function<void(void*, void*)> move_ctor;    // dst <- src
    std::function<void(void*, void*)> move_assign;  // dst <- src
    std::function<void(void*)> dtor;
  };

  const CmptTraits* GetTraits(TypeID ID) const noexcept;

  void Register(const RTDCmptTraits&, TypeID);

  void Deregister(TypeID) noexcept;

 private:
  small_vector<CmptTraits, 16> cmpt_traits;
};
}  // namespace My::MyECS
