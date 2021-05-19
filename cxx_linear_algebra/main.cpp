#include "matrix.hpp"
#include "vector.hpp"

auto matrix_demo() -> void
{
    auto mat     = ta::DynamicMatrix<double> {2, 2};
    mat.at(0, 0) = 1;
    mat.at(1, 1) = 1;
    std::cout << mat << '\n';
    std::cout << mat + mat << '\n';
    // std::cout << mat * mat << '\n';
    std::cout << mat + 2.0 << '\n';
    std::cout << mat - 1.0 << '\n';
    std::cout << mat * 3.0 << '\n';
}

auto vector_demo() -> void
{
    auto vec = ta::DynamicVector<double> {2};
    std::cout << vec << '\n';
}

auto main() -> int
{
    matrix_demo();
    vector_demo();
    return EXIT_SUCCESS;
}