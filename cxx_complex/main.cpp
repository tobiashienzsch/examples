#include <complex>
#include <iostream>

auto main() -> int
{
    auto c = std::complex<double> {2, 3};
    std::cout << "abs(complex" << c << "): " << abs(c) << '\n';
    std::cout << "arg(complex" << c << "): " << arg(c) << '\n';
    std::cout << "norm(complex" << c << "): " << norm(c) << '\n';
    std::cout << "conj(complex" << c << "): " << conj(c) << '\n';
    std::cout << "proj(complex" << c << "): " << proj(c) << '\n';
    return 0;
}