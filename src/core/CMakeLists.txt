ADD_TARGET(
        MODE STATIC
        RET_TARGET_NAME tname
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyECS"
        LIB
        My::MyTemplate_core
        My::MyGraphviz_core
        INC
        "${PROJECT_SOURCE_DIR}/include"
)

TARGET_PRECOMPILE_HEADERS(${tname} PRIVATE "${PROJECT_SOURCE_DIR}/include/MyECS/World.h")