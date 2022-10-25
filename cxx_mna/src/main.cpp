#include <iostream>

#include <mc/spice/spice_circuit.hpp>

auto main(int argc, char const** argv) -> int
{
    if (argc != 2) { return EXIT_FAILURE; }

    auto circuit = mc::loadSpiceCircuit(argv[1]);
    std::cout << circuit << '\n';

    return EXIT_SUCCESS;
}