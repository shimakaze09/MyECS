# MyECS

**My** **E**ntity-**C**omponent-**S**ystem in Unity3D-style

## Environment

- MSVC 16.5.3 +
- C++ 17
- CMake 16.3 +

## Documentation

- [API](doc/API.md) 
- [Comparison with Unity3D ECS](doc/comparison.md) 
- [TODO](doc/todo.md) 

## Example

```c++
#include <MyECS/World.h>

struct Position { float val; };
struct Velocity { float val; };

struct MoverSystem {
    static void OnUpdate(My::Schedule& schedule) {
        schedule.Request(
            [](const Velocity* v, Position* p) {
                p->val += v->val;
            }, "Mover");
    }
};

int main() {
    My::World w;
    w.systemMngr.Register<MoverSystem>();
    w.entityMngr.CreateEntity<Position, Velocity>();
    w.Update();
}
```

**other examples**

- [read/write tag](src/test/01_tag/main.cpp)
- [system update order](src/test/02_order/main.cpp)
- system function with [`Entity`](src/test/03_query_entity/main.cpp), [
  `indexInQuery`](src/test/09_idx_in_query/main.cpp)
- [job function](src/test/08_job/main.cpp)
- [chunk layout optimization with alignment](src/test/05_alignment/main.cpp)
- [parallel with `None` filter](src/test/06_none_parallel/main.cpp)
- [system **overload**](src/test/07_overload/main.cpp)
- [runtime dynamic component and system](src/test/11_runtime_cmpt/main.cpp)
