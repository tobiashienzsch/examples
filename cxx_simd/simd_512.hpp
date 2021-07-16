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

using Reg512f = __m512;
using Reg512i = __m512i;
using Mask512 = std::uint32_t;

MC_FORCE_INLINE auto toMask16(Mask512 mask) noexcept -> __mmask16
{
    return (__mmask16)mask;
}

MC_FORCE_INLINE auto loadValue(float v) noexcept -> Reg512f
{
    return _mm512_set1_ps(v);
}

MC_FORCE_INLINE auto loadValue(int v) noexcept -> Reg512i
{
    return _mm512_set1_epi32(v);
}

MC_FORCE_INLINE auto loadFrom(float const* src) noexcept -> Reg512f
{
    return _mm512_loadu_ps(src);
}

MC_FORCE_INLINE auto maskedLoadFrom(float const* src, Reg512f fill,
    Mask512 mask) noexcept -> Reg512f
{
    return _mm512_mask_loadu_ps(fill, toMask16(mask), src);
}

MC_FORCE_INLINE auto maskedLoadFrom(float const* src, float fill,
    Mask512 mask) noexcept -> Reg512f
{
    return _mm512_mask_loadu_ps(loadValue(fill), toMask16(mask), src);
}

MC_FORCE_INLINE auto maskedStoreTo(float* dest, Reg512f r,
    Mask512 mask) noexcept -> void
{
    _mm512_mask_storeu_ps(dest, toMask16(mask), r);
}

MC_FORCE_INLINE auto storeTo(float* dest, Reg512f r) noexcept -> void
{
    _mm512_storeu_ps(dest, r);
}

MC_FORCE_INLINE auto minimum(Reg512f lhs, Reg512f rhs) -> Reg512f
{
    return _mm512_min_ps(lhs, rhs);
}

MC_FORCE_INLINE auto maximum(Reg512f lhs, Reg512f rhs) -> Reg512f
{
    return _mm512_max_ps(lhs, rhs);
}

MC_FORCE_INLINE auto add(Reg512f lhs, Reg512f rhs) noexcept -> Reg512f
{
    return _mm512_add_ps(lhs, rhs);
}

MC_FORCE_INLINE auto sub(Reg512f lhs, Reg512f rhs) noexcept -> Reg512f
{
    return _mm512_sub_ps(lhs, rhs);
}

MC_FORCE_INLINE auto mul(Reg512f lhs, Reg512f rhs) noexcept -> Reg512f
{
    return _mm512_mul_ps(lhs, rhs);
}

MC_FORCE_INLINE auto fusedMultiplyAdd(Reg512f a, Reg512f b, Reg512f c) noexcept
    -> Reg512f
{
    return _mm512_fmadd_ps(a, b, c);
}

MC_FORCE_INLINE auto blend(Reg512f a, Reg512f b, Mask512 mask) noexcept
    -> Reg512f
{
    return _mm512_mask_blend_ps(toMask16(mask), a, b);
}

/// \brief Reorders the elements in r based on the indices in perm
MC_FORCE_INLINE auto permute(Reg512f r, Reg512i perm) noexcept -> Reg512f
{
    return _mm512_permutexvar_ps(perm, r);
}

/// \brief Permutes the source register and blends the result with the fill
/// register based on the mask
MC_FORCE_INLINE auto maskedPermute(Reg512f fill, Reg512f src, Reg512i perm,
    Mask512 mask) noexcept -> Reg512f
{
    return _mm512_mask_permutexvar_ps(fill, toMask16(mask), perm, src);
}

// clang-format off
template <                                                           
    unsigned A = 0, unsigned B = 0, unsigned C = 0, unsigned D = 0,  
    unsigned E = 0, unsigned F = 0, unsigned G = 0, unsigned H = 0,  
    unsigned I = 0, unsigned J = 0, unsigned K = 0, unsigned L = 0,  
    unsigned M = 0, unsigned N = 0, unsigned O = 0, unsigned P = 0   
>
constexpr auto makeBitMask() -> Mask512 {
    static_assert(                                   
        (A < 2) && (B < 2) && (C < 2) && (D < 2) &&  
        (E < 2) && (F < 2) && (G < 2) && (H < 2) &&  
        (I < 2) && (J < 2) && (K < 2) && (L < 2) &&  
        (M < 2) && (N < 2) && (O < 2) && (P < 2)     
    );
    return (                                           
        (A <<  0) | (B <<  1) | (C <<  2) | (D <<  3) |    
        (E <<  4) | (F <<  5) | (G <<  6) | (H <<  7) |    
        (I <<  8) | (J <<  9) | (K << 10) | (L << 11) |  
        (M << 12) | (N << 13) | (O << 14) | (P << 15)  
    );
}

template <                                                           
    unsigned A, unsigned B, unsigned C, unsigned D,  
    unsigned E, unsigned F, unsigned G, unsigned H,  
    unsigned I, unsigned J, unsigned K, unsigned L,  
    unsigned M, unsigned N, unsigned O, unsigned P   
>
MC_FORCE_INLINE auto makePermutationMap() -> Reg512i {
    static_assert(                                   
        (A < 16) && (B < 16) && (C < 16) && (D < 16) &&  
        (E < 16) && (F < 16) && (G < 16) && (H < 16) &&  
        (I < 16) && (J < 16) && (K < 16) && (L < 16) &&  
        (M < 16) && (N < 16) && (O < 16) && (P < 16)     
    );

    return _mm512_setr_epi32(A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P);  
}
// clang-format on

template <int R>
MC_FORCE_INLINE auto rotate(Reg512f reg) noexcept -> Reg512f
{
    if constexpr ((R % 16) == 0) {
        return reg;
    } else {
        constexpr auto S = (R > 0) ? (16 - (R % 16)) : -R;
        constexpr auto A = (S + 0) % 16;
        constexpr auto B = (S + 1) % 16;
        constexpr auto C = (S + 2) % 16;
        constexpr auto D = (S + 3) % 16;
        constexpr auto E = (S + 4) % 16;
        constexpr auto F = (S + 5) % 16;
        constexpr auto G = (S + 6) % 16;
        constexpr auto H = (S + 7) % 16;
        constexpr auto I = (S + 8) % 16;
        constexpr auto J = (S + 9) % 16;
        constexpr auto K = (S + 10) % 16;
        constexpr auto L = (S + 11) % 16;
        constexpr auto M = (S + 12) % 16;
        constexpr auto N = (S + 13) % 16;
        constexpr auto O = (S + 14) % 16;
        constexpr auto P = (S + 15) % 16;

        return permute(reg, makePermutationMap<A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P>());
    }
}

template <int R>
MC_FORCE_INLINE auto rotateDown(Reg512f reg) noexcept -> Reg512f
{
    static_assert(R >= 0);
    return rotate<-R>(reg);
}

template <int R>
MC_FORCE_INLINE auto rotateUp(Reg512f reg) noexcept -> Reg512f
{
    static_assert(R >= 0);
    return rotate<R>(reg);
}

template <int S>
constexpr auto shiftDownBlendMask() -> Mask512
{
    constexpr auto a = static_cast<unsigned>(16 <= S);
    constexpr auto b = static_cast<unsigned>(15 <= S);
    constexpr auto c = static_cast<unsigned>(14 <= S);
    constexpr auto d = static_cast<unsigned>(13 <= S);
    constexpr auto e = static_cast<unsigned>(12 <= S);
    constexpr auto f = static_cast<unsigned>(11 <= S);
    constexpr auto g = static_cast<unsigned>(10 <= S);
    constexpr auto h = static_cast<unsigned>(9 <= S);
    constexpr auto i = static_cast<unsigned>(8 <= S);
    constexpr auto j = static_cast<unsigned>(7 <= S);
    constexpr auto k = static_cast<unsigned>(6 <= S);
    constexpr auto l = static_cast<unsigned>(5 <= S);
    constexpr auto m = static_cast<unsigned>(4 <= S);
    constexpr auto n = static_cast<unsigned>(3 <= S);
    constexpr auto o = static_cast<unsigned>(2 <= S);
    constexpr auto p = static_cast<unsigned>(1 <= S);
    return makeBitMask<a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p>();
}

template <int S>
MC_FORCE_INLINE auto shiftDown(Reg512f reg) noexcept -> Reg512f
{
    static_assert(S >= 0 && S <= 16);
    return blend(rotateDown<S>(reg), loadValue(0.0f), shiftDownBlendMask<S>());
}

template <int S>
constexpr auto shiftUpBlendMask() -> Mask512
{
    constexpr auto a = static_cast<unsigned>(1 <= S);
    constexpr auto b = static_cast<unsigned>(2 <= S);
    constexpr auto c = static_cast<unsigned>(3 <= S);
    constexpr auto d = static_cast<unsigned>(4 <= S);
    constexpr auto e = static_cast<unsigned>(5 <= S);
    constexpr auto f = static_cast<unsigned>(6 <= S);
    constexpr auto g = static_cast<unsigned>(7 <= S);
    constexpr auto h = static_cast<unsigned>(8 <= S);
    constexpr auto i = static_cast<unsigned>(9 <= S);
    constexpr auto j = static_cast<unsigned>(10 <= S);
    constexpr auto k = static_cast<unsigned>(11 <= S);
    constexpr auto l = static_cast<unsigned>(12 <= S);
    constexpr auto m = static_cast<unsigned>(13 <= S);
    constexpr auto n = static_cast<unsigned>(14 <= S);
    constexpr auto o = static_cast<unsigned>(15 <= S);
    constexpr auto p = static_cast<unsigned>(16 <= S);
    return makeBitMask<a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p>();
}

template <int S>
MC_FORCE_INLINE auto shiftUp(Reg512f reg) noexcept -> Reg512f
{
    static_assert(S >= 0 && S <= 16);
    return blend(rotateUp<S>(reg), loadValue(0.0f), shiftUpBlendMask<S>());
}

template <int S>
MC_FORCE_INLINE auto shiftDownWithCarry(Reg512f lo, Reg512f hi) noexcept
    -> Reg512f
{
    static_assert(S >= 0 && S <= 16);
    return blend(rotateDown<S>(lo), rotateDown<S>(hi), shiftDownBlendMask<S>());
}

template <int S>
MC_FORCE_INLINE auto shiftUpWithCarry(Reg512f lo, Reg512f hi) noexcept
    -> Reg512f
{
    static_assert(S >= 0 && S <= 16);
    return blend(rotateUp<S>(hi), rotateUp<S>(lo), shiftUpBlendMask<S>());
}

template <int S>
MC_FORCE_INLINE auto inPlaceShiftDownWithCarry(Reg512f& lo,
    Reg512f& hi) noexcept -> void
{
    static_assert(S >= 0 && S <= 16);
    constexpr Mask512 zmask = (0xFFFFu >> static_cast<unsigned>(S));
    constexpr Mask512 bmask = ~zmask & 0xFFFFu;
    //   auto perm = makeShiftPermuatation<S, bmask>();
    auto perm = Reg512i {};

    lo = _mm512_permutex2var_ps(lo, perm, hi);
    hi = _mm512_maskz_permutex2var_ps(toMask16(zmask), hi, perm, hi);
}

} // namespace simd