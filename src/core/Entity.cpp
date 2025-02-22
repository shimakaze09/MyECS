//
// Created by Admin on 22/02/2025.
//

#include <MyECS/Entity.h>

using namespace My;
using namespace std;

const vector<tuple<void*, size_t>> Entity::Components() const {
  return archetype->Components(idx);
}

void Entity::Release() noexcept {
  assert(IsAlive());
  archetype->mngr->Release(this);
}

bool Entity::IsAlive() const noexcept {
  return archetype != nullptr;
}

void Entity::AddCommand(const std::function<void()>& command) {
  archetype->mngr->AddCommand(command);
}

My::World* Entity::World() const noexcept {
  assert(IsAlive());
  return archetype->mngr->World();
}
