# MyECS

My Entity-Component-System

## concept

- `System`: function, needs to be associated with an ID (string)
- `Job`: parameterless executable function obtained after preparing data for the system, internally parallel or
  serial
- `Schedule`: stores all tasks and the constraints between tasks. After completing all declarations, a large
  task can be assembled
- `ScheduleRegistrar`: users use it to register the system of the frame