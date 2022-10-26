cmake_minimum_required(VERSION 3.21)
project(cxx_mna VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(mc_conan)

find_package(Eigen3)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
target_sources(${PROJECT_NAME}
    PRIVATE
        src/mc/strings.cpp

        src/mc/spice/spice_capacitor.hpp
        src/mc/spice/spice_capacitor.cpp
        src/mc/spice/spice_circuit.hpp
        src/mc/spice/spice_circuit.cpp
        src/mc/spice/spice_element.hpp
        src/mc/spice/spice_element.cpp
        src/mc/spice/spice_inductor.hpp
        src/mc/spice/spice_inductor.cpp
        src/mc/spice/spice_resistor.hpp
        src/mc/spice/spice_resistor.cpp
        src/mc/spice/spice_voltage_source.hpp
        src/mc/spice/spice_voltage_source.cpp

        src/mc/spice/detail/parse_spice_number.hpp
        src/mc/spice/detail/parse_spice_number.cpp
)