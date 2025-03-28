# Change Log

- 0.14.3
    - `CmptsView`, `SingletonsView` use `Span`
    - API with `const CmptType* types, size_t num` use `Span<CmptType> types` as instead
    - move `include/UECS/detail/_deps/taskflow` to `include/_deps/taskflow`
- 0.14.2
    - Added `RandomAccessor` for accessing components from other entities
        - Enhanced `SystemFunc` with additional member
        - Extended `Schedule::Register*` with new parameter
        - Improved `None Parallel` feature to support `RandomAccessor`
        - Updated `World::GenUpdateFrameGraph` to accommodate `RandomAccessor`
    - Changed default `AccessMode` in `CmptAccessType` from `LATEST` to `WRITE`
    - Modified `World` command buffer layer's type from `size_t` to `int`
- 0.14.1: Removed singleton from `CmptAccessMode`
- 0.14.0: Implemented System Lifecycle