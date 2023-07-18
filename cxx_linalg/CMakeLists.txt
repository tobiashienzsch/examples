cmake_minimum_required(VERSION 3.23)
project(cxx_linalg)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

add_executable(linalg main.cpp)
target_link_libraries(linalg PRIVATE etl)