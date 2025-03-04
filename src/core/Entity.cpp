//
// Created by Admin on 22/02/2025.
//

#include <MyECS/Entity.h>

using namespace My;
using namespace std;

vector<CmptPtr> Entity::Components() const {
  return archetype->Components(idx);
}

void Entity::Release() noexcept {
  archetype->mngr->Release(this);
}

void Entity::AddCommand(const std::function<void()>& command) {
  archetype->mngr->AddCommand(command);
}

My::World* Entity::World() const noexcept {
  return archetype->mngr->World();
}
