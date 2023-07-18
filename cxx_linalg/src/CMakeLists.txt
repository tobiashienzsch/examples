project(etl)

add_library(etl INTERFACE)
target_include_directories(etl INTERFACE ${PROJECT_SOURCE_DIR})
target_compile_definitions(etl
    INTERFACE
        MDSPAN_IMPL_STANDARD_NAMESPACE=etl
        MDSPAN_IMPL_PROPOSED_NAMESPACE=experimental
)