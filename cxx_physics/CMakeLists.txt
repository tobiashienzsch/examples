
cmake_minimum_required(VERSION 3.16)
project(cxx_physics)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} physics_test.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})