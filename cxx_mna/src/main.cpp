
#include <mc/spice/spice_circuit.hpp>

#include <Eigen/Dense>

#include <iostream>

auto main(int argc, char const** argv) -> int
{
    auto A = Eigen::Matrix3d{};
    A << 1, 2, 3, 4, 5, 6, 7, 8, 10;
    std::cout << "Here is the matrix A:\n" << A << '\n';

    auto b = Eigen::Vector3d{};
    b << 3, 3, 4;
    std::cout << "Here is the vector b:\n" << b << '\n';

    auto x = A.colPivHouseholderQr().solve(b);
    std::cout << "The solution is:\n" << x << '\n';
    
    if (argc != 2) { return EXIT_FAILURE; }

    auto circuit = mc::loadSpiceCircuit(argv[1]);
    std::cout << circuit << '\n';

    return EXIT_SUCCESS;

}
