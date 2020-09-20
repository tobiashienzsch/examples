cmake_minimum_required(VERSION 3.15)
project(HeavyMath VERSION 0.1.0)

# Only do these if this is the main project, 
# and not if it is included through add_subdirectory.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # C++ Standard
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Shared Library
    set(BUILD_SHARED_LIBS ON)
    set(CMAKE_CXX_VISIBLITY_PRESET hidden)
    set(CMAKE_VISIBLITY_INLINES_HIDDEN ON)

    # Link-Time Optimization
    include(CheckIPOSupported)
    check_ipo_supported(RESULT COMPILER_SUPPORTS_LTO)
    if(COMPILER_SUPPORTS_LTO)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    if(NOT APPLE)
        set(CMAKE_INSTALL_RPATH $ORIGIN)
    endif(NOT APPLE)

    include(CMakePackageConfigHelpers)    
    write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake VERSION 1.0.0 COMPATIBILITY SameMajorVersion)

    # Compile commands for IDE & tools.
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()    

include(GenerateExportHeader)
include(GNUInstallDirs)    

# Packaging support
set(CPACK_PACKAGE_VENDOR "Tobias Hienzsch")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HeavyMath Summary")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /dist
    /.*build.*
    /.*install.*
    /\\\\.DS_Store
)
include(CPack)

add_library(${PROJECT_NAME} lib.cpp)
generate_export_header(${PROJECT_NAME} 
    EXPORT_FILE_NAME "heavymath_export.hpp"
    INCLUDE_GUARD_NAME "HEAVYMATH_EXPORT_HPP"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT           ${PROJECT_NAME}_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT           ${PROJECT_NAME}_Runtime
            NAMELINK_COMPONENT  ${PROJECT_NAME}_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT           ${PROJECT_NAME}_Development
)