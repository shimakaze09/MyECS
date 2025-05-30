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

## Compiler compatibility

- Clang/LLVM >= 10.0
- GCC >= 10.0
- MSVC >= 1926

> Tested platforms:
>
> - Windows 10: VS2019 16.8.5
>
> - Ubuntu 20: GCC 10.2, Clang 11.0
>
> - MacOS 11.0 : GCC 10.2
>
>   > AppleClang 12 and Clang 11 is not supported

## Documentation

- [changelog](doc/changelog.md)
- [TODO](doc/todo.md)

## Example

```c++
#include <MyECS/MyECS.hpp>

using namespace Smkz::MyECS;

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
  w.entityMngr.cmptTraits.Register<Position, Velocity>();
  w.systemMngr.RegisterAndActivate<MoverSystem>();
  w.entityMngr.Create(Smkz::TypeIDs_of<Position, Velocity>);
  w.Update();
}
```

**other examples**

- [read/write tag](src/test/01_tag/main.cpp)
- [system update order](src/test/02_order/main.cpp)
- [job function](src/test/08_job/main.cpp)
- system function with [`Entity`](src/test/03_query_entity/main.cpp)
- [chunk layout optimization with alignment](src/test/05_alignment/main.cpp)
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
- [change filter](src/test/22_change_filter/main.cpp)