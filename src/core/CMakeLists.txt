ADD_TARGET(
        MODE STATIC
        SOURCE "${PROJECT_SOURCE_DIR}/include/MyECS"
        LIB My::MyDP_core My::MyContainer_core
        INC include
)