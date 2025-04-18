ADD_TARGET(
        TEST
        RET_TARGET_NAME tname
        MODE EXE
        LIB
        My::MyECS_core
)
TARGET_PRECOMPILE_HEADERS(${tname} REUSE_FROM MyECS_core)