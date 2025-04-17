#include <MyECS/SingletonLocator.hpp>

using namespace My::MyECS;
using namespace std;

SingletonLocator::SingletonLocator(std::span<const AccessTypeID> types) {
  for (const auto& type : types) singletonTypes.insert(type);
}

bool SingletonLocator::HasWriteSingletonType() const noexcept {
  for (const auto& type : singletonTypes) {
    if (type.GetAccessMode() == AccessMode::WRITE) return true;
  }
  return false;
}
