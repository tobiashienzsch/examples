cmake_minimum_required(VERSION 3.0.0)
project(cxx_autocorrelation_bitstream VERSION 0.1.0)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)