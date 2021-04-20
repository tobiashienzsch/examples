cmake_minimum_required(VERSION 3.15)
project(cxx_boost_json VERSION 0.1.0)

# Only do these if this is the main project,
# and not if it is included through add_subdirectory.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_program(CCACHE ccache)
    if (CCACHE)
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    endif ()

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(BUILD_SHARED_LIBS OFF)
endif ()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(Boost 1.75 REQUIRED COMPONENTS filesystem json)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::headers Boost::filesystem Boost::json Threads::Threads)

