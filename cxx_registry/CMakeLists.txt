cmake_minimum_required(VERSION 3.0.0)
project(cxx_registry VERSION 0.1.0)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)