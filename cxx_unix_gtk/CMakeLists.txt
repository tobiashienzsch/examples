cmake_minimum_required(VERSION 3.13)
project(cxx_unix_gtk)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtkmm-3.0)

add_executable(file_dialog)
target_sources(file_dialog PRIVATE file_dialog.cpp)
target_link_libraries(file_dialog ${GTK_LIBRARIES})
target_include_directories(file_dialog PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(file_dialog PRIVATE ${GTK_LIBRARY_DIRS})

add_executable(notification)
target_sources(notification PRIVATE notification.cpp)
target_link_libraries(notification ${GTK_LIBRARIES})
target_include_directories(notification PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(notification PRIVATE ${GTK_LIBRARY_DIRS})
