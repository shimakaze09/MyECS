#pragma once

#include <MyGraphviz/MyGraphviz.hpp>
#include <mutex>

#include "CommandBuffer.hpp"
#include "Entity.hpp"
#include "EntityMngr.hpp"
#include "Schedule.hpp"
#include "SystemMngr.hpp"

namespace My::MyECS {
class IListener;

class synchronized_monotonic_buffer_resource
    : public std::pmr::monotonic_buffer_resource {
 public:
  using std::pmr::monotonic_buffer_resource::monotonic_buffer_resource;

  void release() noexcept /* strengthened */ {
    std::lock_guard<std::mutex> guard{mtx};
    this->monotonic_buffer_resource::release();
  }

 protected:
  virtual void* do_allocate(const size_t _Bytes, const size_t _Align) override {
    std::lock_guard<std::mutex> guard{mtx};
    return this->monotonic_buffer_resource::do_allocate(_Bytes, _Align);
  }

 private:
  mutable std::mutex mtx;
};

// SystemMngr + EntityMngr
class World {
  // [relationship]
  // - sync_rsrc --> chunk_unsync_rsrc
  //
  //                 x--> sync_frame_rsrc --> chunk_unsync_frame_rsrc
  //                 |
  // - unsync_rsrc --x
  //                 |
  //                 x--> unsync_frame_rsrc

  std::unique_ptr<std::pmr::synchronized_pool_resource> sync_rsrc;
  std::unique_ptr<std::pmr::unsynchronized_pool_resource> unsync_rsrc;

  std::unique_ptr<synchronized_monotonic_buffer_resource>
      sync_frame_rsrc;  // release every frame
  std::unique_ptr<std::pmr::monotonic_buffer_resource>
      unsync_frame_rsrc;  // release every frame

 public:
  World();
  World(const World&);
  World(World&&) noexcept;
  ~World();

  SystemMngr systemMngr;
  EntityMngr entityMngr;

  // 1. update schedule
  // 2. run job graph for several layers
  // 3. update version
  // 4. clear frame resource
  void Update();

  void AddCommand(std::function<void()> command, int layer);
  void AddCommandBuffer(CommandBuffer cb, int layer);

  // after running Update()
  // you can use graphviz to vistualize the graph
  std::string DumpUpdateJobGraph() const;

  // after running Update()
  // use CmptTraits' registered component name
  MyGraphviz::Graph GenUpdateFrameGraph(int layer = 0) const;

  void Accept(IListener*) const;

  // you can't run several parallel jobs in parallel because there is only an
  // executor

  // Func's argument list:
  // [const] World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // Entity
  // std::size_t indexInQuery
  // <tagged-components>: [const] <Cmpt>*...
  // CmptsView
  template <typename Func>
  CommandBuffer RunEntityJob(Func&&, bool isParallel = true,
                             ArchetypeFilter = {}, CmptLocator = {},
                             SingletonLocator = {});

  // Func's argument list:
  // const World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // Entity
  // std::size_t indexInQuery
  // <tagged-components>: const <Cmpt>*...
  // CmptsView
  // --
  // CmptLocator's Cmpt AccessMode can't be WRITE
  template <typename Func>
  void RunEntityJob(Func&&, bool isParallel = true, ArchetypeFilter = {},
                    CmptLocator = {}, SingletonLocator = {}) const;

  // Func's argument list:
  // [const] World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // std::size_t entityBeginIndexInQuery
  // ChunkView (necessary)
  template <typename Func>
  CommandBuffer RunChunkJob(Func&&, ArchetypeFilter = {},
                            bool isParallel = true, SingletonLocator = {});

  // Func's argument list:
  // const World*
  // {LastFrame|Latest}<Singleton<Cmpt>>
  // SingletonsView
  // std::size_t entityBeginIndexInQuery
  // ChunkView (necessary)
  // --
  // ArchetypeFilter's Cmpt AccessMode can't be WRITE
  template <typename Func>
  void RunChunkJob(Func&&, ArchetypeFilter = {}, bool isParallel = true,
                   SingletonLocator = {}) const;

  synchronized_monotonic_buffer_resource* GetSyncFrameResource() {
    return sync_frame_rsrc.get();
  }
  std::pmr::monotonic_buffer_resource* GetUnsyncFrameResource() {
    return unsync_frame_rsrc.get();
  }
  std::pmr::synchronized_pool_resource* GetSyncResource() {
    return sync_rsrc.get();
  }
  std::pmr::unsynchronized_pool_resource* GetUnsyncResource() {
    return unsync_rsrc.get();
  }

  template <typename T, typename... Args>
  T* SyncNewFrameObject(Args&&... args);
  template <typename T, typename... Args>
  T* UnsyncNewFrameObject(Args&&... args);

  std::uint64_t Version() const noexcept { return version; }

 private:
  bool inRunningJobGraph{false};

  mutable JobExecutor executor;
  Schedule schedule;
  std::uint64_t version{0};

  Job jobGraph;
  std::vector<Job*> jobs;

  // command
  std::map<int, CommandBuffer> lcommandBuffer;
  std::mutex commandBufferMutex;
  void RunCommands(int layer);

  CommandBuffer Run(SystemFunc*);
};
}  // namespace My::MyECS

#include "details/World.inl"
