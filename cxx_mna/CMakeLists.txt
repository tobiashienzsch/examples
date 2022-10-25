cmake_minimum_required(VERSION 3.21)
project(cxx_mna VERSION 0.1.0)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_sources(${PROJECT_NAME}
    PRIVATE
        src/mc/strings.cpp

        src/mc/spice/spice_capacitor.cpp
        src/mc/spice/spice_capacitor.cpp
        src/mc/spice/spice_circuit.hpp
        src/mc/spice/spice_circuit.cpp
        src/mc/spice/spice_element.cpp
        src/mc/spice/spice_element.cpp
        src/mc/spice/spice_inductor.cpp
        src/mc/spice/spice_inductor.cpp
        src/mc/spice/spice_resistor.cpp
        src/mc/spice/spice_resistor.cpp
        src/mc/spice/spice_voltage_source.cpp
        src/mc/spice/spice_voltage_source.cpp
)