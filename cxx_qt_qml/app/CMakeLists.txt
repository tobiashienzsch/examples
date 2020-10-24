project(cxx_qt_qml_app)

add_executable(${PROJECT_NAME})
qt5_add_resources(QT_RESOURCES qml.qrc)
target_sources(${PROJECT_NAME} PRIVATE main.cpp ${QT_RESOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${Qt5Quick_DEFINITIONS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Qml Qt5::Quick) 