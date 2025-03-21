GET_TARGET_NAME(CORE "${PROJECT_SOURCE_DIR}/src/core")
ADD_TARGET(
        MODE EXE
        LIB
        My::MyTemplate_core
        INC_PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)