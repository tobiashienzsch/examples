cmake_minimum_required(VERSION 3.13)
project(cxx_boost_safe_numerics)

find_package(Boost 1.71 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
