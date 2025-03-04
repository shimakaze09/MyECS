ADD_TARGET(
        MODE STATIC
        SOURCE "${PROJECT_SOURCE_DIR}/include/MyECS"
        LIB My::MyTemplate_core My::MyContainer_core
        INC include
)