```

 __  __       _____ ____ ____  
|  \/  |_   _| ____/ ___/ ___| 
| |\/| | | | |  _|| |   \___ \ 
| |  | | |_| | |__| |___ ___) |
|_|  |_|\__, |_____\____|____/ 
        |___/                  

```

[![repo-size](https://img.shields.io/github/languages/code-size/shimakaze09/MyECS?style=flat)](https://github.com/shimakaze09/MyECS/archive/main.zip) [![tag](https://img.shields.io/github/v/tag/shimakaze09/MyECS)](https://github.com/shimakaze09/MyECS/tags) [![license](https://img.shields.io/github/license/shimakaze09/MyECS)](LICENSE)

# MyECS

**My** **E**ntity-**C**omponent-**S**ystem in Unity3D-style

## Environment

- VS 2019
- C++ 17
- CMake 16.3 +

## Documentation

- [changelog](doc/changelog.md)
- [TODO](doc/todo.md)

## Example

```c++
#include <MyECS/World.h>

using namespace My::MyECS;

struct Position { float val; };
struct Velocity { float val; };

struct MoverSystem {
    static void OnUpdate(Schedule& schedule) {
        schedule.RegisterEntityJob(
            [](const Velocity* v, Position* p) {
                p->val += v->val;
            },
            "Mover"
        );
    }
};

int main() {
  World w;
  w.systemMngr.RegisterAndActivate<MoverSystem>();
  w.entityMngr.Create<Position, Velocity>();
  w.Update();
}
```

**other examples**

- [read/write tag](src/test/01_tag/main.cpp)
- [system update order](src/test/02_order/main.cpp)
- [job function](src/test/08_job/main.cpp)
- system function with [`Entity`](src/test/03_query_entity/main.cpp)
- [chunk layout optimization with alignment](src/test/05_alignment/main.cpp)
- [parallel with `None` filter](src/test/06_none_parallel/main.cpp)
- [system **overload**](src/test/07_overload/main.cpp)
- [runtime dynamic component](src/test/11_runtime_cmpt/main.cpp)
- [generate **frame graph** in **Graphviz**](src/test/12_framegraph/main.cpp)
- [performance test](src/test/13_performance/main.cpp)
- [serialize](src/test/14_serialize/main.cpp)
- [chunk job](src/test/15_chunk_job/main.cpp)
- [singleton](src/test/16_singleton/main.cpp)
- [serial execution](src/test/17_serial/main.cpp)
- [world copy](src/test/18_copy/main.cpp)
- [directly run execution](src/test/19_direct_run/main.cpp)
- [system lifecycle](src/test/20_system_lifecycle/main.cpp)
- [random access components](src/test/21_random/main.cpp) 