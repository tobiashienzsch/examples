cmake_minimum_required(VERSION 3.15)
project(cxx_qt_qml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)

add_subdirectory(app)