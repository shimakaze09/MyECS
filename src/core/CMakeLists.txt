ADD_TARGET(
        MODE STATIC
        SOURCE "${PROJECT_SOURCE_DIR}/include/MyECS"
        LIB My::MyContainer_core My::MyTemplate_core
        INC include
)