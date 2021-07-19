#ifndef MATHOPS_COMMON_H
#define MATHOPS_COMMON_H

// File: MathOps_Common.h
// Defines common types for templated MathOps headers (AVX, SSE, scalar).
// Author: Angus F. Hewlett
// Copyright FXpansion Audio UK Ltd. 2014-2017

// This file provides:-
//
// * Base math operations class (which calls specialised functions in the SIMD
// implementation class)
// * A vector float class (with implementation type & interleave factor
// * "typedef" and "using" aliases to import defs from the MATHOPS base class
// (which are required by gcc and llvm-clang)
//

#include <cstdint>

#if WIN32
#define ALIGN_PRE(x) __declspec(align(x))
#define ALIGN_POST(x)
#else
#define ALIGN_PRE(x)
#define ALIGN_POST(x) __attribute__((aligned(x)))
#endif

template<typename simd_t, std::int32_t interleave>
struct alignas(simd_t::alignment) vf_t;

// Basic math operations. Agnostic to data type, interleave factor, and
// implementation.
template<typename simd_t, typename elem_t, int interleave>
struct MathBase
{
    using vf = typename vf_t<simd_t, interleave>;

    // Interleaving function generator for 1-op
    template<typename op_t>
    static vforceinline vf funcgen_1op(const vf& a)
    {
        vf result;
        if (interleave > 0x0) result.m[0x0] = op_t::op(a.m[0x0]);
        if (interleave > 0x1) result.m[0x1] = op_t::op(a.m[0x1]);
        if (interleave > 0x2) result.m[0x2] = op_t::op(a.m[0x2]);
        if (interleave > 0x3) result.m[0x3] = op_t::op(a.m[0x3]);
        if (interleave > 0x4) result.m[0x4] = op_t::op(a.m[0x4]);
        if (interleave > 0x5) result.m[0x5] = op_t::op(a.m[0x5]);
        if (interleave > 0x6) result.m[0x6] = op_t::op(a.m[0x6]);
        if (interleave > 0x7) result.m[0x7] = op_t::op(a.m[0x7]);
        if (interleave > 0x8) result.m[0x8] = op_t::op(a.m[0x8]);
        if (interleave > 0x9) result.m[0x9] = op_t::op(a.m[0x9]);
        if (interleave > 0xA) result.m[0xA] = op_t::op(a.m[0xA]);
        if (interleave > 0xB) result.m[0xB] = op_t::op(a.m[0xB]);
        if (interleave > 0xC) result.m[0xC] = op_t::op(a.m[0xC]);
        if (interleave > 0xD) result.m[0xD] = op_t::op(a.m[0xD]);
        if (interleave > 0xE) result.m[0xE] = op_t::op(a.m[0xE]);
        if (interleave > 0xF) result.m[0xF] = op_t::op(a.m[0xF]);
        return result;
    };

    // Interleaving function generator for 2-op
    template<typename op_t>
    static vforceinline vf funcgen_2op(const vf& a, const vf& b)
    {
        vf result;
        if (interleave > 0x0) result.m[0x0] = op_t::op(a.m[0x0], b.m[0x0]);
        if (interleave > 0x1) result.m[0x1] = op_t::op(a.m[0x1], b.m[0x1]);
        if (interleave > 0x2) result.m[0x2] = op_t::op(a.m[0x2], b.m[0x2]);
        if (interleave > 0x3) result.m[0x3] = op_t::op(a.m[0x3], b.m[0x3]);
        if (interleave > 0x4) result.m[0x4] = op_t::op(a.m[0x4], b.m[0x4]);
        if (interleave > 0x5) result.m[0x5] = op_t::op(a.m[0x5], b.m[0x5]);
        if (interleave > 0x6) result.m[0x6] = op_t::op(a.m[0x6], b.m[0x6]);
        if (interleave > 0x7) result.m[0x7] = op_t::op(a.m[0x7], b.m[0x7]);
        if (interleave > 0x8) result.m[0x8] = op_t::op(a.m[0x8], b.m[0x8]);
        if (interleave > 0x9) result.m[0x9] = op_t::op(a.m[0x9], b.m[0x9]);
        if (interleave > 0xA) result.m[0xA] = op_t::op(a.m[0xA], b.m[0xA]);
        if (interleave > 0xB) result.m[0xB] = op_t::op(a.m[0xB], b.m[0xB]);
        if (interleave > 0xC) result.m[0xC] = op_t::op(a.m[0xC], b.m[0xC]);
        if (interleave > 0xD) result.m[0xD] = op_t::op(a.m[0xD], b.m[0xD]);
        if (interleave > 0xE) result.m[0xE] = op_t::op(a.m[0xE], b.m[0xE]);
        if (interleave > 0xF) result.m[0xF] = op_t::op(a.m[0xF], b.m[0xF]);
        return result;
    };

    // Interleaving function generator for 2-op
    template<typename op_t>
    static vforceinline vf funcgen_3op(const vf& a, const vf& b, const vf& c)
    {
        vf result;
        if (interleave > 0x0)
            result.m[0x0] = op_t::op(a.m[0x0], b.m[0x0], c.m[0x0]);
        if (interleave > 0x1)
            result.m[0x1] = op_t::op(a.m[0x1], b.m[0x1], c.m[0x1]);
        if (interleave > 0x2)
            result.m[0x2] = op_t::op(a.m[0x2], b.m[0x2], c.m[0x2]);
        if (interleave > 0x3)
            result.m[0x3] = op_t::op(a.m[0x3], b.m[0x3], c.m[0x3]);
        if (interleave > 0x4)
            result.m[0x4] = op_t::op(a.m[0x4], b.m[0x4], c.m[0x4]);
        if (interleave > 0x5)
            result.m[0x5] = op_t::op(a.m[0x5], b.m[0x5], c.m[0x5]);
        if (interleave > 0x6)
            result.m[0x6] = op_t::op(a.m[0x6], b.m[0x6], c.m[0x6]);
        if (interleave > 0x7)
            result.m[0x7] = op_t::op(a.m[0x7], b.m[0x7], c.m[0x7]);
        if (interleave > 0x8)
            result.m[0x8] = op_t::op(a.m[0x8], b.m[0x8], c.m[0x8]);
        if (interleave > 0x9)
            result.m[0x9] = op_t::op(a.m[0x9], b.m[0x9], c.m[0x9]);
        if (interleave > 0xA)
            result.m[0xA] = op_t::op(a.m[0xA], b.m[0xA], c.m[0xA]);
        if (interleave > 0xB)
            result.m[0xB] = op_t::op(a.m[0xB], b.m[0xB], c.m[0xB]);
        if (interleave > 0xC)
            result.m[0xC] = op_t::op(a.m[0xC], b.m[0xC], c.m[0xC]);
        if (interleave > 0xD)
            result.m[0xD] = op_t::op(a.m[0xD], b.m[0xD], c.m[0xD]);
        if (interleave > 0xE)
            result.m[0xE] = op_t::op(a.m[0xE], b.m[0xE], c.m[0xE]);
        if (interleave > 0xF)
            result.m[0xF] = op_t::op(a.m[0xF], b.m[0xF], c.m[0xF]);
        return result;
    };

    //  One-op and two-op functions

    // Basics
    static vforceinline vf addps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_add_f>(q1, q2);
    };
    static vforceinline vf subps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_sub_f>(q1, q2);
    };
    static vforceinline vf mulps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_mul_f>(q1, q2);
    };
    static vforceinline vf divps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_div_f>(q1, q2);
    };

    // Minmax
    static vforceinline vf minps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_min_f>(q1, q2);
    };
    static vforceinline vf maxps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_max_f>(q1, q2);
    };

    // Booleans
    static vforceinline vf andps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_and_f>(q1, q2);
    };
    static vforceinline vf andnps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_andn_f>(q1, q2);
    };
    static vforceinline vf orps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_or_f>(q1, q2);
    };
    static vforceinline vf xorps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_xor_f>(q1, q2);
    };
    static vforceinline vf notps(const vf& q1)
    {
        return funcgen_1op<typename simd_t::op_not_f>(q1);
    };

    // Compares
    static vforceinline vf cmpgeps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_cmpge_f>(q1, q2);
    };
    static vforceinline vf cmpgtps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_cmpgt_f>(q1, q2);
    };
    static vforceinline vf cmpleps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_cmple_f>(q1, q2);
    };
    static vforceinline vf cmpltps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_cmplt_f>(q1, q2);
    };
    static vforceinline vf cmpeqps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_cmpeq_f>(q1, q2);
    };
    static vforceinline vf cmpneps(const vf& q1, const vf& q2)
    {
        return funcgen_2op<typename simd_t::op_cmpne_f>(q1, q2);
    };

    // Misc
    static vforceinline vf rcpps(const vf& q1)
    {
        return funcgen_1op<typename simd_t::op_rcp_f>(q1);
    };
    static vforceinline vf absps(const vf& q1)
    {
        return funcgen_1op<typename simd_t::op_abs_f>(q1);
    };
    static vforceinline vf zerops() { return assign_single(0.f); };
    static vforceinline vf set1ps(float q1) { return assign_single(q1); };

    static vforceinline vf sub_if_greater_ps(const vf& q1, const vf& q2,
                                             const vf& q3)
    {
        return funcgen_3op<typename simd_t::op_sub_if_greater_f>(q1, q2, q3);
    };

    // Composites

    // Clip
    static vforceinline vf clipps(const vf& val, const vf& lo_lim,
                                  const vf& hi_lim)
    {
        return maxps(lo_lim, minps(val, hi_lim));
    };

    // Cube
    static vforceinline vf cubeps(const vf& q1)
    {
        return mulps(q1, mulps(q1, q1));
    };

    // Clip01
    static vforceinline vf clip01ps(const vf& q1)
    {
        return maxps(minps(q1, set1ps(1.f)), 0.f);
    };

    // Mask
    static vforceinline vf maskps(const vf& maskee, const vf& conditional_mask)
    {
        return andps(maskee, conditional_mask);
    };

    // Assignment - copy
    static vforceinline void assign_copy(vf& result, const vf& a)
    {
        if (interleave > 0x0) result.m[0x0] = a.m[0x0];
        if (interleave > 0x1) result.m[0x1] = a.m[0x1];
        if (interleave > 0x2) result.m[0x2] = a.m[0x2];
        if (interleave > 0x3) result.m[0x3] = a.m[0x3];
        if (interleave > 0x4) result.m[0x4] = a.m[0x4];
        if (interleave > 0x5) result.m[0x5] = a.m[0x5];
        if (interleave > 0x6) result.m[0x6] = a.m[0x6];
        if (interleave > 0x7) result.m[0x7] = a.m[0x7];
        if (interleave > 0x8) result.m[0x8] = a.m[0x8];
        if (interleave > 0x9) result.m[0x9] = a.m[0x9];
        if (interleave > 0xA) result.m[0xA] = a.m[0xA];
        if (interleave > 0xB) result.m[0xB] = a.m[0xB];
        if (interleave > 0xC) result.m[0xC] = a.m[0xC];
        if (interleave > 0xD) result.m[0xD] = a.m[0xD];
        if (interleave > 0xE) result.m[0xE] = a.m[0xE];
        if (interleave > 0xF) result.m[0xF] = a.m[0xF];
    };

    // Assignment - array of elements
    static vforceinline vf assign_array(const elem_t (&a)[interleave])
    {
        vf result;
        if (interleave > 0x0) result.m[0x0] = a[0x0];
        if (interleave > 0x1) result.m[0x1] = a[0x1];
        if (interleave > 0x2) result.m[0x2] = a[0x2];
        if (interleave > 0x3) result.m[0x3] = a[0x3];
        if (interleave > 0x4) result.m[0x4] = a[0x4];
        if (interleave > 0x5) result.m[0x5] = a[0x5];
        if (interleave > 0x6) result.m[0x6] = a[0x6];
        if (interleave > 0x7) result.m[0x7] = a[0x7];
        if (interleave > 0x8) result.m[0x8] = a[0x8];
        if (interleave > 0x9) result.m[0x9] = a[0x9];
        if (interleave > 0xA) result.m[0xA] = a[0xA];
        if (interleave > 0xB) result.m[0xB] = a[0xB];
        if (interleave > 0xC) result.m[0xC] = a[0xC];
        if (interleave > 0xD) result.m[0xD] = a[0xD];
        if (interleave > 0xE) result.m[0xE] = a[0xE];
        if (interleave > 0xF) result.m[0xF] = a[0xF];
        return result;
    };

    // Assignment - single float
    static vforceinline vf assign_single(float f)
    {
        elem_t a = simd_t::op_set_f::op(f);
        vf result;
        if (interleave > 0x0) result.m[0x0] = a;
        if (interleave > 0x1) result.m[0x1] = a;
        if (interleave > 0x2) result.m[0x2] = a;
        if (interleave > 0x3) result.m[0x3] = a;
        if (interleave > 0x4) result.m[0x4] = a;
        if (interleave > 0x5) result.m[0x5] = a;
        if (interleave > 0x6) result.m[0x6] = a;
        if (interleave > 0x7) result.m[0x7] = a;
        if (interleave > 0x8) result.m[0x8] = a;
        if (interleave > 0x9) result.m[0x9] = a;
        if (interleave > 0xA) result.m[0xA] = a;
        if (interleave > 0xB) result.m[0xB] = a;
        if (interleave > 0xC) result.m[0xC] = a;
        if (interleave > 0xD) result.m[0xD] = a;
        if (interleave > 0xE) result.m[0xE] = a;
        if (interleave > 0xF) result.m[0xF] = a;
        return result;
    };
};

// Vector float type (vf_t)
//
template<typename simd_t, std::int32_t interleave>
struct alignas(simd_t::alignment) vf_t : simd_t
{
    using vec_elem_f = typename simd_t::vec_elem_f;

    // Data storage - (interleave) elements of SIMD vectors
    alignas(simd_t::alignment) vec_elem_f m[interleave];

    // Default ctor doesn't initialize. standard c++.
    vforceinline vf_t() {};

    // Debug: check alignment
    // if (((((size_t)(char*)this) % simd_t::alignment) != 0) ||
    // ((((size_t)(char*)&f) % simd_t::alignment) != 0))	printf("!");

    // Copy constructors and assignment operators
    // Can take:- another vf_t, a single vec_elem_f element which is copied to
    // all, a single float copied to all.
    vforceinline vf_t(const vf_t& f) { *this = f; };
    vforceinline vf_t(const vec_elem_f (&f)[interleave]) { *this = f; };
    vforceinline vf_t(const float& f) { *this = f; };

    vforceinline vf_t& operator=(const vf_t& f)
    {
        simd_t::assign_copy(*this, f);
        return *this;
    };

    vforceinline vf_t& operator=(const vec_elem_f (&f)[interleave])
    {
        *this = simd_t::assign_array(f);
        return *this;
    };

    vforceinline vf_t& operator=(const float f)
    {
        *this = simd_t::assign_single(f);
        return *this;
    };

    // Math operations taking float on lhs
    friend vforceinline vf_t operator*(float lhs, const vf_t& rhs)
    {
        vf_t tmp = vf_t::set1ps(lhs) * rhs;
        return tmp;
    };
    friend vforceinline vf_t operator+(float lhs, const vf_t& rhs)
    {
        vf_t tmp = vf_t::set1ps(lhs) + rhs;
        return tmp;
    };
    friend vforceinline vf_t operator-(float lhs, const vf_t& rhs)
    {
        vf_t tmp = vf_t::set1ps(lhs) - rhs;
        return tmp;
    };

    // Basic maths: +,-,* (no divide, it's slow. if you want divide, figure out
    // a faster way).
    vforceinline vf_t operator+(const vf_t& m2) const
    {
        return vf_t::addps(*this, m2);
    };
    vforceinline vf_t operator-(const vf_t& m2) const
    {
        return vf_t::subps(*this, m2);
    };
    vforceinline vf_t operator*(const vf_t& m2) const
    {
        return vf_t::mulps(*this, m2);
    };

    // Comparison operators (return a vector bitmask: a full lane is 111 if
    // true, 000 if false.
    vforceinline vf_t operator>(const vf_t& m2) const
    {
        return vf_t::cmpgtps(*this, m2);
    };
    vforceinline vf_t operator>=(const vf_t& m2) const
    {
        return vf_t::cmpgeps(*this, m2);
    };
    vforceinline vf_t operator<(const vf_t& m2) const
    {
        return vf_t::cmpltps(*this, m2);
    };
    vforceinline vf_t operator<=(const vf_t& m2) const
    {
        return vf_t::cmpleps(*this, m2);
    };
    vforceinline vf_t operator==(const vf_t& m2) const
    {
        return vf_t::cmpeqps(*this, m2);
    };
    vforceinline vf_t operator!=(const vf_t& m2) const
    {
        return vf_t::cmpneps(*this, m2);
    };

    // Logical operators
    vforceinline vf_t operator&(const vf_t& m2) const
    {
        return vf_t::andps(*this, m2);
    };
    vforceinline vf_t& operator!() const { return notps(*this); };

    // In-place maths: +,-,* with assignment, vector versions.
    vforceinline vf_t& operator+=(const vf_t& f)
    {
        *this = vf_t::addps(*this, f);
        return *this;
    };
    vforceinline vf_t& operator-=(const vf_t& f)
    {
        *this = vf_t::subps(*this, f);
        return *this;
    };
    vforceinline vf_t& operator*=(const vf_t& f)
    {
        *this = vf_t::mulps(*this, f);
        return *this;
    };

    // In-place maths: +,-,* with assignment, scalar constant versions
    vforceinline vf_t& operator+=(const float f)
    {
        vf_t tmp_f = f;
        *this      = vf_t::addps(*this, tmp_f);
        return *this;
    };
    vforceinline vf_t& operator-=(const float f)
    {
        vf_t tmp_f = f;
        *this      = vf_t::subps(*this, tmp_f);
        return *this;
    };
    vforceinline vf_t& operator*=(const float f)
    {
        vf_t tmp_f = f;
        *this      = vf_t::mulps(*this, tmp_f);
        return *this;
    };

    // Clip

    // In-place clip with vector bounds
    vforceinline void clip(const vf_t& lo_lim, const vf_t& hi_lim)
    {
        *this = vf_t::minps(vf_t::maxps(*this, lo_lim), hi_lim);
    };

    // In-place clip with scalar bounds
    vforceinline void clip(const float lo_lim, const float hi_lim)
    {
        vf_t hi_lim_v = hi_lim;
        vf_t lo_lim_v = lo_lim;
        *this         = vf_t::minps(vf_t::maxps(*this, lo_lim_v), hi_lim_v);
    };

    // In-place clip with value input & vector bounds
    vforceinline void clip_v(const vf_t& val, const vf_t& lo_lim,
                             const vf_t& hi_lim)
    {
        *this = vf_t::minps(vf_t::maxps(val, lo_lim), hi_lim);
    };

    // In-place clip with value input & scalar bounds
    vforceinline void clip_v(const vf_t& val, const float lo_lim,
                             const float hi_lim)
    {
        vf_t hi_lim_v = hi_lim;
        vf_t lo_lim_v = lo_lim;
        *this         = vf_t::minps(vf_t::maxps(val, lo_lim_v), hi_lim_v);
    };
} ALIGN_POST(simd_t::alignment);

// This provides typedef aliases from base class to make function definitions
// cleaner (required for gcc), and 'using' import definitions required by
// LLVM-Clang
#define DEFINE_MATHOPS_IMPORTS                                                 \
    using SampleInputStream  = typename TIOAdapter::SampleInputStream;         \
    using SampleOutputStream = typename TIOAdapter::SampleOutputStream;        \
    using IOAdapterImpl      = TIOAdapter;                                     \
    using vf                 = typename TIOAdapter::vf;                        \
    using TIOAdapter::minps;                                                   \
    using TIOAdapter::maxps;                                                   \
    using TIOAdapter::addps;                                                   \
    using TIOAdapter::mulps;                                                   \
    using TIOAdapter::subps;                                                   \
    using TIOAdapter::divps;                                                   \
    using TIOAdapter::rcpps;                                                   \
    using TIOAdapter::absps;                                                   \
    using TIOAdapter::zerops;                                                  \
    using TIOAdapter::maskps;                                                  \
    using TIOAdapter::notps;                                                   \
    using TIOAdapter::num_elem;                                                \
    using TIOAdapter::cubeps;                                                  \
    using TIOAdapter::clip01ps;                                                \
    using TIOAdapter::clipps;                                                  \
    using TIOAdapter::cmpgeps;                                                 \
    using TIOAdapter::cmpgtps;                                                 \
    using TIOAdapter::cmpleps;                                                 \
    using TIOAdapter::cmpltps;                                                 \
    using TIOAdapter::cmpeqps;                                                 \
    using TIOAdapter::cmpneps;                                                 \
    using TIOAdapter::sub_if_greater_ps;

#endif
