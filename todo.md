# TODO

## core

### important

- [x] Entity += version
- [x] CmptTypeSet hash
- [x] CmptType
- [x] EntityQuery
- [x] Query Entity
- [x] alignment
- [x] `Entity` alias table: `Entity::idx -> (Archetype, idx)`
- [x] `Entity` as special `Component` stored in `Chunk`
- [x] `None` Parallel
- [x] System Override (dynamic `None`)
- [x] Archetype += RuntimeCmptTraits
- [x] empty argument `SystemFunc` as job
- [x] index in Query
    - [x] SystemFunc support
    - [x] get entity num of query
- [x] instantiate

### unimportant

- [x] exception: invalid `Entity`
- [ ] string-driven API
- [ ] batch create
- [ ] lock filter change

### maybe deprecate

- [ ] shared component
- [ ] system group

## tool

- [ ] SysFuncGraph dump
- [ ] serialize

## detail

- [x] remove `EntityMngr::ai2ei`
- [x] Archetype share `Pool<Chunk>`
- [ ] simplify `Schedule`
- [ ] parallel `Schedule`
- [x] `constexpr SystemFunc::HashCode()`
- [x] cache `CmptIDSet`'s hashcode
- [x] store `EntityMngr` and `SystemMngr` instead `World` in `Schedule` 