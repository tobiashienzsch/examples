cmake_minimum_required(VERSION 3.23)
project(cxx-sycl C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(IntelDPCPP REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE sycl)

add_executable(${PROJECT_NAME}-info info.cpp)
# target_link_libraries(${PROJECT_NAME}-info PRIVATE sycl)
