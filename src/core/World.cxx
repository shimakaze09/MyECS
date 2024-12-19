//
// Created by Admin on 19/12/2024.
//

#include <MyECS/World.hxx>

using namespace My;

World::World() : mngr{&sysMngr, this} {}

void World::Update(bool dump) {
  SystemManager::Table table;
  sysMngr.GenTaskflow(table);
  if (dump)
    table.finalSys.dump(std::cout);
  executor.run(table.finalSys).wait();
}