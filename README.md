# MyECS

A custom Entity-Component-System framework for game development.

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
- [ ] System lifecycle
    - [ ] start, stop
    - [ ] creat, destroy
- [ ] System Override (dynamic `None`)
- [x] Archetype += RuntimeCmptTraits
- [ ] exception

### Tool

- [ ] SysFuncGraph dump