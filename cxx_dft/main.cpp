#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <complex>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <span>
#include <vector>

template<typename T>
auto discreteFourierTransform(std::span<std::complex<T> const> input,
                              std::span<std::complex<T>> output) -> void
{
    auto const N = input.size();
    auto const K = N;

    for (std::size_t k = 0; k < K; ++k)
    {
        auto tmp = std::complex<T> {};
        for (std::size_t n = 0; n < N; ++n)
        {
            auto const part = ((T {2} * std::numbers::pi_v<T>) / N) * k * n;
            auto const real = static_cast<T>(std::cos(part));
            auto const imag = -static_cast<T>(std::sin(part));
            tmp += input[n] * std::complex<T> {real, imag};
        }
        output[k] = tmp;
    }
}

auto main() -> int
{
    using Float = float;

    constexpr auto N  = std::size_t {1'000};
    constexpr auto Nd = static_cast<Float>(N);
    auto signal       = std::vector<std::complex<Float>> {};
    signal.reserve(N);

    auto const sigK     = Float {3};
    auto const sigPhase = Float {0};

    for (auto x = std::size_t {}; x < N; ++x)
    {
        auto const xd = static_cast<Float>(x);
        auto sample
            = std::cos((Float {2} * std::numbers::pi_v<Float> / Nd) * sigK * xd
                       + sigPhase);
        signal.emplace_back(sample, Float {0});
    }

    auto frequencies = std::vector<std::complex<Float>> {};
    frequencies.resize(N);
    discreteFourierTransform<Float>(signal, frequencies);

    std::cout << "k\t" << std::setw(12) << "real\t" << std::setw(12)
              << "imag\n";
    for (auto i = std::size_t {}; i < 6; ++i)
    {
        std::cout << i << '\t' << std::setw(12) << frequencies[i].real() / Nd
                  << '\t' << std::setw(12) << frequencies[i].imag() / Nd
                  << '\n';
    }
    return EXIT_SUCCESS;
}