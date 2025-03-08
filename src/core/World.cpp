//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

using namespace My;
using namespace std;

void World::Update() {
  schedule.Clear();
  for (auto job : jobs)
    jobPool.Recycle(job);
  jobs.clear();
  jobGraph.clear();

  for (const auto& [id, onUpdate] : systemMngr.onUpdateMap)
    onUpdate(schedule);
  auto graph = schedule.GenSysFuncGraph();

  unordered_map<SystemFunc*, JobHandle> table;

  for (const auto& [func, adjVs] : graph.GetAdjList()) {
    auto job = jobPool.Request(func->Name());
    jobs.push_back(job);
    if (!func->IsJob())
      entityMngr.GenJob(job, func);
    else
      job->emplace([func = func]() {
        (*func)(Entity::Invalid(), size_t_invalid,
                RTDCmptsView{nullptr, nullptr});
      });
    table[func] = jobGraph.composed_of(*job);
  }

  for (const auto& [v, adjVs] : graph.GetAdjList()) {
    auto vJob = table[v];
    for (auto adjV : adjVs)
      vJob.precede(table[adjV]);
  }

  executor.run(jobGraph).wait();

  entityMngr.RunCommands();
}

string World::DumpUpdateJobGraph() const {
  return jobGraph.dump();
}
