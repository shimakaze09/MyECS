# MyECS

A custom Entity-Component-System framework for game development.

## Compare with Unity ECS [->](compare.md)

MyECS's primary reference object is Unity's ECS.

Read [compare.md](compare.md) for details

## TODOs

### Core

- [x] EntityData: version
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
- [ ] remove `EntityMngr::ai2ei`
- [ ] copy
- [ ] batch create

### Tool

- [ ] SysFuncGraph dump