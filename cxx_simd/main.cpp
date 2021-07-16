#include "simd_256.hpp"

#include <cstdio>

auto print(char const* prefix, simd::Reg256f reg) -> void
{
    std::printf("%15s: %2.0f", prefix, reg.m256_f32[0]);
    for (std::size_t i = 1; i < 8; ++i) {
        std::printf(", %2.0f", reg.m256_f32[i]);
    }
    std::printf("\n");
}

auto main() -> int
{
    float const data[32] = {
        0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f,         //
        8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f,   //
        16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, //
        24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, //
    };

    auto lo = simd::loadFrom(std::begin(data));
    auto hi = simd::loadFrom(std::next(data, 8));

    print("lo", lo);
    print("hi", hi);
    print("rotateDown<2>", simd::rotateDown<2>(lo));
    print("rotateUp<2>", simd::rotateUp<2>(lo));
    // print("shiftDown<2>", simd::shiftDown<2>(lo));
    // print(simd::shiftUp<2>(lo));
    // print(simd::shiftDownWithCarry<4>(lo, hi));
    // print(simd::shiftUpWithCarry<3>(lo, hi));

    // auto r1 = simd::rotateDown<2>(lo);
    // auto r2 = simd::rotateUp<2>(lo);
    // auto r3 = simd::shiftDown<2>(lo);
    // auto r4 = simd::shiftUp<2>(lo);
    // auto r5 = simd::shiftDownWithCarry<4>(lo, hi);
    // //   auto f6 = simd::shiftUpWithCarry<3>(lo, hi);

    // auto result = simd::add(r1, simd::sub(r2, simd::mul(r3, simd::add(r4, r5))));
    // // return static_cast<int>(result[5]);
}