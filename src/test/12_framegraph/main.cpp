//
// Created by Admin on 8/03/2025.
//

#include <MyECS/World.h>

#include <iostream>

using namespace My::MyECS;
using namespace My;
using namespace std;

struct A {};

struct B {};

struct C {};

struct D {};

struct E {};

struct F {};

struct G {};

class MySystem : public System {
 public:
  using System::System;

  virtual void OnUpdate(Schedule& schedule) override {
    ArchetypeFilter filter;
    filter.all = {CmptType::Of<D>};
    filter.any = {CmptType::Of<E>, CmptType::Of<F>};
    filter.none = {CmptType::Of<G>};
    schedule.RegisterEntityJob([](LastFrame<A> a, Write<B> b, Latest<C> c) {},
                               "System Func", filter);
  }
};

int main() {
  World w;
  w.systemMngr.Register<MySystem>();

  RTDCmptTraits::Instance()
      .RegisterName(CmptType::Of<A>, "A")
      .RegisterName(CmptType::Of<B>, "B")
      .RegisterName(CmptType::Of<C>, "C")
      .RegisterName(CmptType::Of<D>, "D")
      .RegisterName(CmptType::Of<E>, "E")
      .RegisterName(CmptType::Of<F>, "F")
      .RegisterName(CmptType::Of<G>, "G");

  w.entityMngr.Create<A, B, C, D, E>();

  w.Update();

  cout << w.DumpUpdateJobGraph() << endl;
  cout << w.GenUpdateFrameGraph().Dump() << endl;

  return 0;
}
