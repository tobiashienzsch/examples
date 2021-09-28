cmake_minimum_required(VERSION 3.13)
project(cxx_boost_ipc)

find_package(Threads REQUIRED)
find_package(Boost 1.71 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost Threads::Threads)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME} PRIVATE Shlwapi)
else()
  find_library(LIB_RT rt)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_RT})
endif()