//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

using namespace My;

World::World() : mngr{&sysMngr, this} {}

void World::Update(bool dump) {
  SystemMngr::Table table;
  sysMngr.GenTaskflow(table);
  if (dump)
    table.finalSys.dump(std::cout);
  executor.run(table.finalSys).wait();
}
