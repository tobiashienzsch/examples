#include <cstdint>
#include <iterator>
#include <type_traits>

#ifdef __OPTIMIZE__
#include <immintrin.h>
#define MC_FORCE_INLINE inline __attribute__((__always_inline__))
#else
#define __OPTIMIZE__
#include <immintrin.h>
#undef __OPTIMIZE__
#define MC_FORCE_INLINE inline
#endif

namespace simd {

using Reg256f = __m256;
using Reg256i = __m256i;
using Mask256 = std::uint16_t;

MC_FORCE_INLINE auto toMask8(Mask256 mask) noexcept -> __mmask8
{
    return (__mmask8)mask;
}

MC_FORCE_INLINE auto loadValue(float v) noexcept -> Reg256f
{
    return _mm256_set1_ps(v);
}

MC_FORCE_INLINE auto loadValue(int v) noexcept -> Reg256i
{
    return _mm256_set1_epi32(v);
}

MC_FORCE_INLINE auto loadFrom(float const* src) noexcept -> Reg256f
{
    return _mm256_loadu_ps(src);
}

MC_FORCE_INLINE auto maskedLoadFrom(float const* src, Reg256f fill,
    Mask256 mask) noexcept -> Reg256f
{
    return _mm256_mask_expandloadu_ps(fill, toMask8(mask), src);
    // return _mm256_maskload_ps(src, toMask8(mask));
}

MC_FORCE_INLINE auto maskedLoadFrom(float const* src, float fill,
    Mask256 mask) noexcept -> Reg256f
{
    return maskedLoadFrom(src, loadValue(fill), toMask8(mask));
}

MC_FORCE_INLINE auto maskedStoreTo(float* dest, Reg256f r,
    Mask256 mask) noexcept -> void
{
    _mm256_mask_storeu_ps(dest, toMask8(mask), r);
}

MC_FORCE_INLINE auto storeTo(float* dest, Reg256f r) noexcept -> void
{
    _mm256_storeu_ps(dest, r);
}

MC_FORCE_INLINE auto minimum(Reg256f lhs, Reg256f rhs) -> Reg256f
{
    return _mm256_min_ps(lhs, rhs);
}

MC_FORCE_INLINE auto maximum(Reg256f lhs, Reg256f rhs) -> Reg256f
{
    return _mm256_max_ps(lhs, rhs);
}

MC_FORCE_INLINE auto add(Reg256f lhs, Reg256f rhs) noexcept -> Reg256f
{
    return _mm256_add_ps(lhs, rhs);
}

MC_FORCE_INLINE auto sub(Reg256f lhs, Reg256f rhs) noexcept -> Reg256f
{
    return _mm256_sub_ps(lhs, rhs);
}

MC_FORCE_INLINE auto mul(Reg256f lhs, Reg256f rhs) noexcept -> Reg256f
{
    return _mm256_mul_ps(lhs, rhs);
}

MC_FORCE_INLINE auto fusedMultiplyAdd(Reg256f a, Reg256f b, Reg256f c) noexcept
    -> Reg256f
{
    return _mm256_fmadd_ps(a, b, c);
}

MC_FORCE_INLINE auto blend(Reg256f a, Reg256f b, Mask256 mask) noexcept
    -> Reg256f
{
    return _mm256_mask_blend_ps(toMask8(mask), a, b);
}

/// \brief Reorders the elements in r based on the indices in perm
MC_FORCE_INLINE auto permute(Reg256f r, Reg256i perm) noexcept -> Reg256f
{
    return _mm256_permutexvar_ps(perm, r);
}

/// \brief Permutes the source register and blends the result with the fill
/// register based on the mask
MC_FORCE_INLINE auto maskedPermute(Reg256f fill, Reg256f src, Reg256i perm,
    Mask256 mask) noexcept -> Reg256f
{
    return _mm256_mask_permutexvar_ps(fill, toMask8(mask), perm, src);
}

// clang-format off
template <                                                           
    unsigned A = 0, unsigned B = 0, unsigned C = 0, unsigned D = 0,  
    unsigned E = 0, unsigned F = 0, unsigned G = 0, unsigned H = 0
>
constexpr auto makeBitMask() -> Mask256 {
    static_assert(                                   
        (A < 2) && (B < 2) && (C < 2) && (D < 2) &&  
        (E < 2) && (F < 2) && (G < 2) && (H < 2)
    );
    return (                                           
        (A <<  0) | (B <<  1) | (C <<  2) | (D <<  3) |    
        (E <<  4) | (F <<  5) | (G <<  6) | (H <<  7)  
    );
}

template <                                                           
    unsigned A, unsigned B, unsigned C, unsigned D,  
    unsigned E, unsigned F, unsigned G, unsigned H   
>
MC_FORCE_INLINE auto makePermutationMap() -> Reg256i {
    static_assert(                                   
        (A < 16) && (B < 16) && (C < 16) && (D < 16) &&  
        (E < 16) && (F < 16) && (G < 16) && (H < 16)   
    );

    return _mm256_setr_epi32(A,B,C,D,E,F,G,H);  
}
// clang-format on

template <int R>
MC_FORCE_INLINE auto rotate(Reg256f reg) noexcept -> Reg256f
{
    if constexpr ((R % 8) == 0) {
        return reg;
    } else {
        constexpr auto S = (R > 0) ? (8 - (R % 8)) : -R;
        constexpr auto A = (S + 0) % 8;
        constexpr auto B = (S + 1) % 8;
        constexpr auto C = (S + 2) % 8;
        constexpr auto D = (S + 3) % 8;
        constexpr auto E = (S + 4) % 8;
        constexpr auto F = (S + 5) % 8;
        constexpr auto G = (S + 6) % 8;
        constexpr auto H = (S + 7) % 8;

        return permute(reg, makePermutationMap<A, B, C, D, E, F, G, H>());
    }
}

template <int R>
MC_FORCE_INLINE auto rotateDown(Reg256f reg) noexcept -> Reg256f
{
    static_assert(R >= 0);
    return rotate<-R>(reg);
}

template <int R>
MC_FORCE_INLINE auto rotateUp(Reg256f reg) noexcept -> Reg256f
{
    static_assert(R >= 0);
    return rotate<R>(reg);
}

template <int S>
constexpr auto shiftDownBlendMask() -> Mask256
{
    constexpr auto i = static_cast<unsigned>(8 <= S);
    constexpr auto j = static_cast<unsigned>(7 <= S);
    constexpr auto k = static_cast<unsigned>(6 <= S);
    constexpr auto l = static_cast<unsigned>(5 <= S);
    constexpr auto m = static_cast<unsigned>(4 <= S);
    constexpr auto n = static_cast<unsigned>(3 <= S);
    constexpr auto o = static_cast<unsigned>(2 <= S);
    constexpr auto p = static_cast<unsigned>(1 <= S);
    return makeBitMask<i, j, k, l, m, n, o, p>();
}

template <int S>
MC_FORCE_INLINE auto shiftDown(Reg256f reg) noexcept -> Reg256f
{
    static_assert(S >= 0 && S <= 8);
    auto const a    = rotateDown<S>(reg);
    auto const b    = loadValue(0.0f);
    auto const mask = shiftDownBlendMask<S>();
    return blend(a, b, mask);
    // return Reg256f {};
}

template <int S>
constexpr auto shiftUpBlendMask() -> Mask256
{
    constexpr auto a = static_cast<unsigned>(1 <= S);
    constexpr auto b = static_cast<unsigned>(2 <= S);
    constexpr auto c = static_cast<unsigned>(3 <= S);
    constexpr auto d = static_cast<unsigned>(4 <= S);
    constexpr auto e = static_cast<unsigned>(5 <= S);
    constexpr auto f = static_cast<unsigned>(6 <= S);
    constexpr auto g = static_cast<unsigned>(7 <= S);
    constexpr auto h = static_cast<unsigned>(8 <= S);
    return makeBitMask<a, b, c, d, e, f, g, h>();
}

template <int S>
MC_FORCE_INLINE auto shiftUp(Reg256f reg) noexcept -> Reg256f
{
    static_assert(S >= 0 && S <= 8);
    return blend(rotateUp<S>(reg), loadValue(0.0f), shiftUpBlendMask<S>());
}

template <int S>
MC_FORCE_INLINE auto shiftDownWithCarry(Reg256f lo, Reg256f hi) noexcept
    -> Reg256f
{
    static_assert(S >= 0 && S <= 8);
    return blend(rotateDown<S>(lo), rotateDown<S>(hi), shiftDownBlendMask<S>());
}

template <int S>
MC_FORCE_INLINE auto shiftUpWithCarry(Reg256f lo, Reg256f hi) noexcept
    -> Reg256f
{
    static_assert(S >= 0 && S <= 8);
    return blend(rotateUp<S>(hi), rotateUp<S>(lo), shiftUpBlendMask<S>());
}

}