# Change Log

- 0.14.4
    - Added single `CmptType` API to EntityMngr
    - Added template APIs for `SystemTraits` and `SystemMngr`
    - Changed component constraint from `static_assert(std::is_copy_constructible_v<Cmpt>)` to
      `std::is_copy_constructible_v<Cmpt> || std::is_constructible_v<Cmpt, Cmpt&>`
- 0.14.3
    - Updated `CmptsView` and `SingletonsView` to use `Span`
    - Replaced APIs using `const CmptType* types, size_t num` with `Span<CmptType> types`
    - Moved `include/UECS/detail/_deps/taskflow` to `include/_deps/taskflow`
- 0.14.2
    - Added `RandomAccessor` for random access to other entities' components
        - Added member to `SystemFunc`
        - Added parameter to `Schedule::Register*`
        - Updated feature `None Parallel` to support `RandomAccessor`
        - Updated `World::GenUpdateFrameGraph` to support `RandomAccessor`
    - Changed default `AccessMode` in `CmptAccessType` from `LATEST` to `WRITE`
    - Changed `World` command buffer layer's type from `size_t` to `int`
- 0.14.1: Removed singleton from `CmptAccessMode`
- 0.14.0: Implemented System Lifecycle