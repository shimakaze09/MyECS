#include "MyECS/World.hxx"

#include <iostream>
#include <random>

using namespace std;
using namespace My;

struct velocity {
  float v{0.f};
};

struct position {
  float x{0.f};
};

int main() {
  constexpr size_t N = 100000;
  World w;
  set<Entity*> entities;

  default_random_engine engine;
  uniform_real_distribution uniform(0.f, 1.f);

  for (size_t i = 0; i < N; ++i) {
    auto e = w.CreateEntity<velocity, position>();
    e->Get<velocity>()->v = uniform(engine);
    e->Get<position>()->x = uniform(engine);
    entities.insert(e);
  }

  auto sys = [](velocity* v, position* p) {
    p->x += v->v;
  };

  w.Each(sys);

  return 0;
}