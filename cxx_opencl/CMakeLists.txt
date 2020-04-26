
cmake_minimum_required(VERSION 3.16)
project(cxx_opencl)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE CL_TARGET_OPENCL_VERSION=220)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)
