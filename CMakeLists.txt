CMAKE_MINIMUM_REQUIRED(VERSION 3.20 FATAL_ERROR)

PROJECT(MyECS VERSION 0.16.1)
MESSAGE(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")

INCLUDE(cmake/InitMyCMake.cmake)
INIT_MYCMAKE(VERSION 0.6.4)

INIT_PROJECT()

ADD_DEP(MyTemplate 0.7.3)
ADD_DEP(MyGraphviz 0.2.2)
ADD_DEP(MySmallFlat 0.2.3)

ADD_SUB_DIRS_REC(include)
ADD_SUB_DIRS_REC(src)

EXPORT_TARGETS(
        TARGET
        DIRECTORIES
        "include"
)