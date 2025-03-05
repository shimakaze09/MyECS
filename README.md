# MyECS

A custom Entity-Component-System framework in Unity-style

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
            }, "MoverSystem");
    }
};

int main() {
    My::World w;
    w.systemMngr.Register<MoverSystem>();

    for (size_t i = 0; i < 10; i++)
        w.entityMngr.CreateEntity<>();

    w.Update();
}
```

## Compare with Unity ECS

MyECS's primary reference project is Unity's ECS.

Read [compare.md](compare.md) for details.

## TODOs

### Core

- [x] ~~EntityData: version~~
- [x] CmptIDSet hash
- [x] CmptType
- [x] EntityQuery
- [x] Query Entity
- [x]  ~~SystemMngr += ScheduleRegistrar~~
- [x] alignment
- [x] `Entity` alias table: `Entity::idx -> (Archetype, idx)`
- [x] `Entity` as special `Component` stored in `Chunk`
- [x] `None` Parallel
- [x] System Override (dynamic `None`)
- [x] Archetype += RuntimeCmptTraits
- [ ] index in Query
- [ ] Shared Component
- [ ] system group

### non-core

- [ ] exception
- [ ] return
- [x] remove `EntityMngr::ai2ei`
- [ ] copy
- [ ] batch create
- [ ] Archetype share `Pool<Chunk>`

### Tool

- [ ] SysFuncGraph dump