//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>
#include <iostream>

struct velocity {
  float value{0.f};
};

struct position {
  float value{0.f};
};

void Print(const My::World& w) {
  w.Each([](const position* p) { std::cout << p->value << std::endl; });
  w.ParallelEach([](const velocity* v) { std::cout << v->value << std::endl; });
}

int main() {
  My::World w;

  for (size_t i = 0; i < 10; i++) {
    auto [entity, v, p] = w.CreateEntity<velocity, position>();
    v->value = static_cast<float>(i);
  }

  float deltaT = 0.033f;

  Print(w);

  return 0;
}
