cmake_minimum_required(VERSION 3.21)
project(cxx_raytracing C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cxx_raytracing "src/main.cpp")
target_sources(cxx_raytracing
    PRIVATE
        "src/image.cpp"
        "src/image.hpp"
        "src/pixel_rgb.cpp"
        "src/pixel_rgb.hpp"
        "src/ray.cpp"
        "src/ray.hpp"
        "src/sphere.cpp"
        "src/sphere.hpp"
        "src/vec3.cpp"
        "src/vec3.hpp"
)

target_compile_definitions(cxx_raytracing
    PRIVATE
        _CRT_SECURE_NO_WARNINGS=1
)