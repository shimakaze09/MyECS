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

Read [todo.md](todo.md) for details.