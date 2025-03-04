# MyECS

A custom Entity-Component-System framework for game development.

## Concepts

- **System**: Functions that are identified by a unique string ID
- **Job**: Parameter-less executable functions created after preparing data for a system, which can run either in
  parallel or sequentially
- **Schedule**: Stores all jobs and their dependency relationships; once all declarations are complete, it can be
  assembled into a comprehensive task
- **ScheduleRegistrar**: Interface through which users register systems for the current frame

## TODO

- [ ] EntityBase -> Entity
- [ ] EntityQuery
- [ ] Query Entity
- [ ] SystemMngr += ScheduleRegistrar