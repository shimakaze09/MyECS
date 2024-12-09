#include "MyECS/core/World.hxx"

#include <iostream>
#include <set>

using namespace My;
using namespace std;

struct velocity {
  velocity() = default;

  ~velocity() { cout << "velocity release " << this << endl; }

  velocity(float v) : v(v) {};
  float v{0.f};
};

struct position {
  ~position() { cout << "position release " << this << endl; }

  float x{0.f};
};

int main() {
  World w;
  set<Entity*> entities;

  auto [e] = w.CreateEntity<>();
  e->Attach<velocity>();
  e->Detach<velocity>();
  e->Attach<position, velocity>();
  e->Detach<position, velocity>();
  for (size_t i = 0; i < 10; ++i)
    w.CreateEntity<>();

  for (size_t i = 0; i < 100000; i++) {
    auto [e] = w.CreateEntity<>();
    entities.insert(e);
  }

  for (size_t i = 0; i < 100000; i++) {
    (*entities.begin())->Release();
    entities.erase(entities.begin());
  }

  // [invalid]
  // size_t i = 0;
  // w.Each([&i]() {
  //   cout << "i: " << i << endl;
  //   i++;
  // });

  // [ invalid ]
  // w.CreateEntity<velocity, velocity>();

  return 0;
}