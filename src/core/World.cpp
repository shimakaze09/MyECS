//
// Created by Admin on 22/02/2025.
//

#include <MyECS/World.h>

#include <MyECS/IListener.h>

#include "SysFuncGraph.h"

using namespace My::MyECS;
using namespace My;
using namespace std;

World::World(const World& w)
    : systemMngr{w.systemMngr, this}, entityMngr{w.entityMngr} {}

World::World(World&& w) noexcept
    : systemMngr{std::move(w.systemMngr), this},
      entityMngr{std::move(w.entityMngr)} {}

World::~World() {
  systemMngr.Clear();
}

void World::Update() {
  inRunningJobGraph = true;

  schedule.Clear();
  for (auto* job : jobs)
    jobPool.Recycle(job);
  jobs.clear();
  jobGraph.clear();

  for (auto id : systemMngr.GetActiveSystemsIDs())
    systemMngr.Update(id, schedule);

  for (auto& [layer, scheduleCommands] : schedule.commandBuffer) {
    auto& worldCommands = commandBuffer[layer];
    for (auto& command : scheduleCommands) {
      worldCommands.emplace_back(
          [this, command = std::move(command)]() { command(this); });
    }
  }
  schedule.commandBuffer.clear();

  const auto graph = schedule.GenSysFuncGraph();

  unordered_map<SystemFunc*, JobHandle> table;

  for (const auto& [func, adjVs] : graph.GetAdjList()) {
    auto* job = jobPool.Request(func->Name());
    jobs.push_back(job);
    entityMngr.AutoGen(this, job, func);
    table.emplace(func, jobGraph.composed_of(*job));
  }

  for (const auto& [v, adjVs] : graph.GetAdjList()) {
    auto vJob = table.at(v);
    for (auto* adjV : adjVs)
      vJob.precede(table.at(adjV));
  }

  executor.run(jobGraph).wait();
  inRunningJobGraph = false;

  RunCommands();
}

string World::DumpUpdateJobGraph() const {
  return jobGraph.dump();
}

void World::Run(SystemFunc* sys) {
  if (sys->IsParallel()) {
    assert(!inRunningJobGraph);
    Job job;
    entityMngr.AutoGen(this, &job, sys);
    executor.run(job).wait();
  } else
    entityMngr.AutoGen(this, nullptr, sys);
}

// after running Update
MyGraphviz::Graph World::GenUpdateFrameGraph() const {
  MyGraphviz::Graph graph("Update Frame Graph", true);

  graph.RegisterGraphNodeAttr("style", "filled")
      .RegisterGraphNodeAttr("fontcolor", "white")
      .RegisterGraphNodeAttr("fontname", "consolas");

  auto& registry = graph.GetRegistry();

  auto& subgraph_cmpt = graph.GenSubgraph("Component Nodes");
  auto& subgraph_singleton = graph.GenSubgraph("Singleton Nodes");
  auto& subgraph_sys = graph.GenSubgraph("System Function Nodes");

  auto& subgraph_lastframe = graph.GenSubgraph("LastFrame Edges");
  auto& subgraph_write = graph.GenSubgraph("Write Edges");
  auto& subgraph_latest = graph.GenSubgraph("Latest Edges");

  auto& subgraph_order = graph.GenSubgraph("Order Edges");

  auto& subgraph_all = graph.GenSubgraph("All Edges");
  auto& subgraph_any = graph.GenSubgraph("Any Edges");
  auto& subgraph_none = graph.GenSubgraph("None Edges");

  subgraph_cmpt.RegisterGraphNodeAttr("shape", "ellipse")
      .RegisterGraphNodeAttr("color", "#6597AD");

  subgraph_singleton.RegisterGraphNodeAttr("shape", "ellipse")
      .RegisterGraphNodeAttr("color", "#BFB500");

  subgraph_sys.RegisterGraphNodeAttr("shape", "box")
      .RegisterGraphNodeAttr("color", "#F79646");

  subgraph_lastframe.RegisterGraphEdgeAttr("color", "#60C5F1");
  subgraph_write.RegisterGraphEdgeAttr("color", "#F47378");
  subgraph_latest.RegisterGraphEdgeAttr("color", "#6BD089");

  subgraph_order.RegisterGraphEdgeAttr("color", "#00A2E8");

  subgraph_all.RegisterGraphEdgeAttr("style", "dashed")
      .RegisterGraphEdgeAttr("color", "#C785C8")
      .RegisterGraphEdgeAttr("arrowhead", "crow");

  subgraph_any.RegisterGraphEdgeAttr("style", "dashed")
      .RegisterGraphEdgeAttr("color", "#C785C8")
      .RegisterGraphEdgeAttr("arrowhead", "diamond");

  subgraph_none.RegisterGraphEdgeAttr("style", "dashed")
      .RegisterGraphEdgeAttr("color", "#C785C8")
      .RegisterGraphEdgeAttr("arrowhead", "odot");

  unordered_set<CmptType> cmptTypes;
  unordered_set<CmptType> singletonTypes;
  unordered_map<CmptType, size_t> cmptType2idx;
  unordered_map<size_t, size_t> sysFuncHashcode2idx;

  auto queryCmptName = [this](CmptType type) -> string {
    auto cmptName = entityMngr.cmptTraits.Nameof(type);
    return cmptName.empty() ? std::to_string(type.HashCode())
                            : string{cmptName};
  };

  for (const auto& [hash, sysFunc] : schedule.sysFuncs) {
    for (auto cmptType : sysFunc->entityQuery.locator.CmptAccessTypes())
      cmptTypes.insert(cmptType);
    for (auto cmptType : sysFunc->entityQuery.filter.all)
      cmptTypes.insert(cmptType);
    for (auto cmptType : sysFunc->entityQuery.filter.any)
      cmptTypes.insert(cmptType);
    for (auto cmptType : sysFunc->entityQuery.filter.none)
      cmptTypes.insert(cmptType);
    for (auto singleton : sysFunc->singletonLocator.SingletonTypes())
      singletonTypes.insert(singleton);
  }

  for (const auto& cmptType : cmptTypes) {
    auto cmptIdx = registry.RegisterNode(queryCmptName(cmptType));
    cmptType2idx.emplace(cmptType, cmptIdx);
    subgraph_cmpt.AddNode(cmptIdx);
  }

  for (const auto& singletonType : singletonTypes) {
    auto cmptIdx = registry.RegisterNode(queryCmptName(singletonType));
    cmptType2idx.emplace(singletonType, cmptIdx);
    subgraph_singleton.AddNode(cmptIdx);
  }

  for (const auto& [hash, sysFunc] : schedule.sysFuncs) {
    auto sysIdx = registry.RegisterNode(sysFunc->Name());
    sysFuncHashcode2idx.emplace(hash, sysIdx);

    subgraph_sys.AddNode(sysIdx);

    for (const auto& cmptType :
         sysFunc->entityQuery.locator.CmptAccessTypes()) {
      size_t edgeIdx;
      switch (cmptType.GetAccessMode()) {
        case AccessMode::LAST_FRAME:
          edgeIdx = registry.RegisterEdge(cmptType2idx.at(cmptType), sysIdx);
          subgraph_lastframe.AddEdge(edgeIdx);
          break;
        case AccessMode::WRITE:
          edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(cmptType));
          subgraph_write.AddEdge(edgeIdx);
          break;
        case AccessMode::LATEST:
          edgeIdx = registry.RegisterEdge(cmptType2idx.at(cmptType), sysIdx);
          subgraph_latest.AddEdge(edgeIdx);
          break;
        default:
          assert(false);
          break;
      }
    }

    const auto& filter = sysFunc->entityQuery.filter;
    if (sysFunc->GetMode() == SystemFunc::Mode::Chunk) {
      // filter's <All> and <Any> components are treat as r/w
      for (const auto& cmptType : filter.all) {
        auto cmptIdx = cmptType2idx.at(cmptType);
        size_t edgeIdx;
        switch (cmptType.GetAccessMode()) {
          case AccessMode::LAST_FRAME:
            edgeIdx = registry.RegisterEdge(cmptType2idx.at(cmptType), sysIdx);
            subgraph_lastframe.AddEdge(edgeIdx);
            break;
          case AccessMode::WRITE:
            edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(cmptType));
            subgraph_write.AddEdge(edgeIdx);
            break;
          case AccessMode::LATEST:
            edgeIdx = registry.RegisterEdge(cmptType2idx.at(cmptType), sysIdx);
            subgraph_latest.AddEdge(edgeIdx);
            break;
          default:
            assert(false);
            break;
        }
      }
      for (const auto& cmptType : filter.any) {
        auto cmptIdx = cmptType2idx.at(cmptType);
        size_t edgeIdx;
        switch (cmptType.GetAccessMode()) {
          case AccessMode::LAST_FRAME:
            edgeIdx = registry.RegisterEdge(cmptType2idx.at(cmptType), sysIdx);
            subgraph_lastframe.AddEdge(edgeIdx);
            break;
          case AccessMode::WRITE:
            edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(cmptType));
            subgraph_write.AddEdge(edgeIdx);
            break;
          case AccessMode::LATEST:
            edgeIdx = registry.RegisterEdge(cmptType2idx.at(cmptType), sysIdx);
            subgraph_latest.AddEdge(edgeIdx);
            break;
          default:
            assert(false);
            break;
        }
      }
    } else {
      for (const auto& cmptType : filter.all) {
        auto cmptIdx = cmptType2idx.at(cmptType);
        if (registry.IsRegisteredEdge(sysIdx, cmptIdx))
          continue;
        auto edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(cmptType));
        subgraph_all.AddEdge(edgeIdx);
      }
      for (const auto& cmptType : filter.any) {
        auto cmptIdx = cmptType2idx.at(cmptType);
        if (registry.IsRegisteredEdge(sysIdx, cmptIdx))
          continue;
        auto edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(cmptType));
        subgraph_any.AddEdge(edgeIdx);
      }
    }
    for (const auto& cmptType : filter.none) {
      auto cmptIdx = cmptType2idx.at(cmptType);
      if (registry.IsRegisteredEdge(sysIdx, cmptIdx))
        continue;
      auto edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(cmptType));
      subgraph_none.AddEdge(edgeIdx);
    }

    for (const auto& singleton : sysFunc->singletonLocator.SingletonTypes()) {
      size_t edgeIdx;
      switch (singleton.GetAccessMode()) {
        case AccessMode::LAST_FRAME:
          edgeIdx = registry.RegisterEdge(cmptType2idx.at(singleton), sysIdx);
          subgraph_lastframe.AddEdge(edgeIdx);
          break;
        case AccessMode::WRITE:
          edgeIdx = registry.RegisterEdge(sysIdx, cmptType2idx.at(singleton));
          subgraph_write.AddEdge(edgeIdx);
          break;
        case AccessMode::LATEST:
          edgeIdx = registry.RegisterEdge(cmptType2idx.at(singleton), sysIdx);
          subgraph_latest.AddEdge(edgeIdx);
          break;
        default:
          assert(false);
          break;
      }
    }
  }

  for (const auto& [from, to] : schedule.sysFuncOrder) {
    auto fromIdx = sysFuncHashcode2idx.at(from);
    auto toIdx = sysFuncHashcode2idx.at(to);
    auto edgeIdx = registry.RegisterEdge(fromIdx, toIdx);
    subgraph_order.AddEdge(edgeIdx);
  }

  return graph;
}

void World::Accept(IListener* listener) const {
  listener->EnterWorld(this);
  entityMngr.Accept(listener);
  listener->ExistWorld(this);
}

void World::AddCommand(std::function<void()> command, size_t layer) {
  assert(inRunningJobGraph);
  std::lock_guard<std::mutex> guard(commandBufferMutex);
  commandBuffer[layer].push_back(std::move(command));
}

void World::RunCommands() {
  for (const auto& [layer, commands] : commandBuffer) {
    for (const auto& command : commands)
      command();
  }
  commandBuffer.clear();
}
