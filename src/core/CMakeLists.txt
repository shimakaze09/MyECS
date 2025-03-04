ADD_TARGET(
        MODE STATIC
        SOURCE "${PROJECT_SOURCE_DIR}/include/MyECS"
        LIB My::MyTemplate_core My::MyBL_core
        INC include
)