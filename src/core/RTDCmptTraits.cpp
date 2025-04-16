#include <MyECS/RTDCmptTraits.h>

using namespace My;
using namespace My::MyECS;

RTDCmptTraits::RTDCmptTraits() { Register<Entity>(); }

RTDCmptTraits::RTDCmptTraits(const RTDCmptTraits& other)
    : sizeofs{other.sizeofs},
      trivials{other.trivials},
      alignments{other.alignments},
      default_constructors{other.default_constructors},
      copy_constructors{other.copy_constructors},
      move_constructors{other.move_constructors},
      move_assignments{other.move_assignments},
      destructors{other.destructors} {
  for (const auto& [id, name] : other.names) {
    char* buffer =
        (char*)rsrc.allocate((name.size() + 1) * sizeof(char), alignof(char));
    std::memcpy(buffer, name.data(), name.size() * sizeof(char));
    buffer[name.size()] = 0;
    names.emplace(id, std::string_view{buffer, name.size()});
    buffer += name.size() + 1;
  }
}

RTDCmptTraits& RTDCmptTraits::operator=(const RTDCmptTraits& rhs) {
  sizeofs = rhs.sizeofs;
  trivials = rhs.trivials;
  alignments = rhs.alignments;
  default_constructors = rhs.default_constructors;
  copy_constructors = rhs.copy_constructors;
  move_constructors = rhs.move_constructors;
  move_assignments = rhs.move_assignments;
  destructors = rhs.destructors;

  for (const auto& [id, name] : rhs.names) {
    char* buffer =
        (char*)rsrc.allocate((name.size() + 1) * sizeof(char), alignof(char));
    std::memcpy(buffer, name.data(), name.size() * sizeof(char));
    buffer[name.size()] = 0;
    names.emplace(id, std::string_view{buffer, name.size()});
    buffer += name.size() + 1;
  }
  return *this;
}

bool RTDCmptTraits::IsTrivial(TypeID type) const {
  return trivials.contains(type);
}

std::size_t RTDCmptTraits::Sizeof(TypeID type) const {
  auto target = sizeofs.find(type);
  assert(target != sizeofs.end());
  return target->second;
}

std::size_t RTDCmptTraits::Alignof(TypeID type) const {
  auto target = alignments.find(type);

  return target != alignments.end() ? target->second : default_alignment;
}

void RTDCmptTraits::DefaultConstruct(TypeID type, void* cmpt) const {
  auto target = default_constructors.find(type);

  if (target != default_constructors.end()) target->second(cmpt);
}

void RTDCmptTraits::CopyConstruct(TypeID type, void* dst, void* src) const {
  auto target = copy_constructors.find(type);

  if (target != copy_constructors.end())
    target->second(dst, src);
  else
    memcpy(dst, src, Sizeof(type));
}

void RTDCmptTraits::MoveConstruct(TypeID type, void* dst, void* src) const {
  auto target = move_constructors.find(type);

  if (target != move_constructors.end())
    target->second(dst, src);
  else
    memcpy(dst, src, Sizeof(type));
}

void RTDCmptTraits::MoveAssign(TypeID type, void* dst, void* src) const {
  auto target = move_assignments.find(type);

  if (target != move_assignments.end())
    target->second(dst, src);
  else
    memcpy(dst, src, Sizeof(type));
}

void RTDCmptTraits::Destruct(TypeID type, void* cmpt) const {
  auto target = destructors.find(type);
  if (target != destructors.end()) target->second(cmpt);
}

std::string_view RTDCmptTraits::Nameof(TypeID type) const {
  auto target = names.find(type);
  if (target != names.end())
    return target->second;
  else
    return {};
}

RTDCmptTraits& RTDCmptTraits::RegisterSize(TypeID type, std::size_t size) {
  sizeofs.emplace(type, size);
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterAlignment(TypeID type,
                                                std::size_t alignment) {
  alignments.emplace(type, alignment);
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterDefaultConstructor(
    TypeID type, std::function<void(void*)> f) {
  default_constructors.emplace(type, std::move(f));
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterCopyConstructor(
    TypeID type, std::function<void(void*, void*)> f) {
  copy_constructors.emplace(type, std::move(f));
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterMoveConstructor(
    TypeID type, std::function<void(void*, void*)> f) {
  move_constructors.emplace(type, std::move(f));
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterMoveAssignment(
    TypeID type, std::function<void(void*, void*)> f) {
  move_assignments.emplace(type, std::move(f));
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterDestructor(TypeID type,
                                                 std::function<void(void*)> f) {
  destructors.emplace(type, std::move(f));
  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterName(Type type) {
  auto target = names.find(type);
  if (target != names.end()) {
    assert(type.Is(target->second));
    return *this;
  }

  auto* buffer = (char*)rsrc.allocate(
      (type.GetName().size() + 1) * sizeof(char), alignof(char));
  std::memcpy(buffer, type.GetName().data(),
              type.GetName().size() * sizeof(char));
  buffer[type.GetName().size()] = 0;
  names.emplace(type.GetID(), std::string_view{buffer, type.GetName().size()});

  return *this;
}

RTDCmptTraits& RTDCmptTraits::RegisterTrivial(TypeID type) {
  trivials.insert(type);
  return *this;
}

RTDCmptTraits& RTDCmptTraits::Deregister(TypeID type) noexcept {
  names.erase(type);
  trivials.erase(type);
  sizeofs.erase(type);
  alignments.erase(type);
  default_constructors.erase(type);
  copy_constructors.erase(type);
  move_constructors.erase(type);
  move_assignments.erase(type);
  destructors.erase(type);
  return *this;
}

RTDCmptTraits& RTDCmptTraits::Clear() {
  names.clear();
  trivials.clear();
  sizeofs.clear();
  alignments.clear();
  default_constructors.clear();
  copy_constructors.clear();
  move_constructors.clear();
  move_assignments.clear();
  destructors.clear();
  return *this;
}
