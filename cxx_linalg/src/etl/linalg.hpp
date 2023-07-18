#pragma once

#include <etl/mdspan.hpp>

#include <cassert>
#include <complex>
#include <concepts>
#include <type_traits>

#define ETL_PRECONDITION(x) assert((x))

namespace etl::linalg {

template<typename Triangle, typename StorageOrder>
struct layout_blas_packed;

namespace detail {
template<typename T>
struct is_mdspan : std::false_type
{};

template<typename T, typename Extents, typename Layout, typename Accessor>
struct is_mdspan<mdspan<T, Extents, Layout, Accessor>> : std::true_type
{};

template<typename T>
concept in_vector = is_mdspan<T>::value && T::rank() == 1;

template<typename T>
concept out_vector
    = is_mdspan<T>::value && T::rank() == 1
   && std::same_as<std::remove_const_t<typename T::element_type>, typename T::element_type>
   && T::is_always_unique();

template<typename T>
concept inout_vector
    = is_mdspan<T>::value && T::rank() == 1
   && std::same_as<std::remove_const_t<typename T::element_type>, typename T::element_type>
   && T::is_always_unique();

template<typename T>
concept in_matrix = is_mdspan<T>::value && T::rank() == 2;

template<typename T>
concept out_matrix
    = is_mdspan<T>::value && T::rank() == 2
   && std::
          is_same_v<std::remove_const_t<typename T::element_type>, typename T::element_type>
   && T::is_always_unique();

template<typename T>
concept inout_matrix
    = is_mdspan<T>::value && T::rank() == 2
   && std::
          is_same_v<std::remove_const_t<typename T::element_type>, typename T::element_type>
   && T::is_always_unique();

// template <typename T>
// concept possibly_packed_inout_matrix =
//     is_mdspan<T>::value && T::rank() == 2 &&
//     std::is_same_v<std::remove_const_t<typename T::element_type>,
//                    typename T::element_type> &&
//     (T::is_always_unique() ||
//      std::is_same_v<typename T::layout_type, layout_blas_packed>);

template<typename T>
concept in_object = is_mdspan<T>::value && (T::rank() == 1 || T::rank() == 2);

template<typename T>
concept out_object
    = is_mdspan<T>::value && (T::rank() == 1 || T::rank() == 2)
   && std::
          is_same_v<std::remove_const_t<typename T::element_type>, typename T::element_type>
   && T::is_always_unique();

template<typename T>
concept inout_object
    = is_mdspan<T>::value && (T::rank() == 1 || T::rank() == 2)
   && std::
          is_same_v<std::remove_const_t<typename T::element_type>, typename T::element_type>
   && T::is_always_unique();

template<typename... Ts>
using common_size_type_t = std::common_type_t<typename Ts::size_type...>;

}  // namespace detail

struct column_major_t
{
    explicit column_major_t() = default;
};

inline constexpr auto column_major = column_major_t{};

struct row_major_t
{
    explicit row_major_t() = default;
};

inline constexpr auto row_major = row_major_t{};

struct upper_triangle_t
{
    explicit upper_triangle_t() = default;
};

inline constexpr auto upper_triangle = upper_triangle_t{};

struct lower_triangle_t
{
    explicit lower_triangle_t() = default;
};

inline constexpr auto lower_triangle = lower_triangle_t{};

struct implicit_unit_diagonal_t
{
    explicit implicit_unit_diagonal_t() = default;
};

inline constexpr auto implicit_unit_diagonal = implicit_unit_diagonal_t{};

struct explicit_diagonal_t
{
    explicit explicit_diagonal_t() = default;
};

inline constexpr auto explicit_diagonal = explicit_diagonal_t{};

template<detail::inout_object InOutObj1, detail::inout_object InOutObj2>
    requires(InOutObj1::rank() == InOutObj1::rank())
constexpr auto swap_elements(InOutObj1 x, InOutObj2 y) -> void
{
    ETL_PRECONDITION(x.extents() == y.extents());

    using size_type = detail::common_size_type_t<InOutObj1, InOutObj2>;

    if constexpr (InOutObj1::rank() == 1) {
        for (size_type i{0}; i < x.extent(0); ++i) { std::swap(x[i], y[i]); }
    } else {
        static_assert(InOutObj1::rank() == 2);
        for (size_type i{0}; i < x.extent(0); ++i) {
            for (size_type j{0}; j < x.extent(1); ++j) { std::swap(x[i, j], y[i, j]); }
        }
    }
}

template<typename Scalar, detail::inout_object InOutObj>
constexpr auto scale(Scalar alpha, InOutObj x) -> void
{
    using size_type = typename InOutObj::size_type;

    if constexpr (InOutObj::rank() == 1) {
        for (size_type i{0}; i < x.extent(0); ++i) { x[i] = x[i] * alpha; }
    } else {
        static_assert(InOutObj::rank() == 2);
        for (size_type i{0}; i < x.extent(0); ++i) {
            for (size_type j{0}; j < x.extent(1); ++j) { x[i, j] = x[i, j] * alpha; }
        }
    }
}

template<detail::in_object InObj, detail::out_object OutObj>
    requires(InObj::rank() == OutObj::rank())
constexpr auto copy(InObj x, OutObj y) -> void
{
    ETL_PRECONDITION(x.extents() == y.extents());

    using size_type = detail::common_size_type_t<InObj, OutObj>;

    if constexpr (InObj::rank() == 1) {
        for (size_type i{0}; i < x.extent(0); ++i) { y[i] = x[i]; }
    } else {
        static_assert(InObj::rank() == 2);
        for (size_type i{0}; i < x.extent(0); ++i) {
            for (size_type j{0}; j < x.extent(1); ++j) { y[i, j] = x[i, j]; }
        }
    }
}

template<detail::in_object InObj1, detail::in_object InObj2, detail::out_object OutObj>
    requires(InObj1::rank() == OutObj::rank() and InObj2::rank() == OutObj::rank())
constexpr auto add(InObj1 x, InObj2 y, OutObj z) -> void
{
    ETL_PRECONDITION(x.extents() == y.extents());

    using size_type = detail::common_size_type_t<InObj1, InObj2, OutObj>;

    if constexpr (OutObj::rank() == 1) {
        for (size_type row{0}; row < x.extent(0); ++row) { z[row] = x[row] + y[row]; }
    } else {
        static_assert(OutObj::rank() == 2);
        for (size_type row{0}; row < x.extent(0); ++row) {
            for (size_type col{0}; col < x.extent(1); ++col) {
                z[row, col] = x[row, col] + y[row, col];
            }
        }
    }
}

template<detail::in_vector InVec, typename Scalar>
constexpr auto vector_two_norm(InVec v, Scalar init) noexcept -> Scalar
{
    auto sum = init;
    for (typename InVec::size_type i{0}; i < v.extent(0); ++i) {
        auto const val = v[i];
        sum += (val * val);
    }
    return std::sqrt(sum);
}

template<detail::in_vector InVec>
constexpr auto vector_two_norm(InVec v) noexcept -> typename InVec::value_type
{
    return vector_two_norm(v, typename InVec::value_type{});
}

template<detail::in_vector InVec, typename Scalar>
constexpr auto vector_abs_sum(InVec v, Scalar init) noexcept -> Scalar
{
    auto sum = init;
    for (typename InVec::size_type i{0}; i < v.extent(0); ++i) { sum += std::abs(v[i]); }
    return sum;
}

template<detail::in_vector InVec>
constexpr auto vector_abs_sum(InVec v) noexcept -> typename InVec::value_type
{
    return vector_abs_sum(v, typename InVec::value_type{});
}

template<detail::in_vector InVec>
constexpr auto idx_abs_max(InVec v) -> typename InVec::size_type
{
    auto idx   = std::numeric_limits<typename InVec::size_type>::max();
    auto max_v = std::numeric_limits<typename InVec::value_type>::min();

    for (typename InVec::size_type i{0}; i < v.extent(0); ++i) {
        if (auto const val = std::abs(v[i]); val > max_v) {
            idx   = i;
            max_v = val;
        }
    }

    return idx;
}

template<detail::in_matrix InMat, typename Scalar>
constexpr auto matrix_frob_norm(InMat A, Scalar init) noexcept -> Scalar
{
    auto result = init;
    for (typename InMat::size_type row{0}; row < A.extent(0); ++row) {
        for (typename InMat::size_type col{0}; col < A.extent(1); ++col) {
            result += std::abs(A[row, col]);
        }
    }
    return std::sqrt(result);
}

template<detail::in_matrix InMat, detail::in_vector InVec, detail::out_vector OutVec>
constexpr auto matrix_vector_product(InMat A, InVec x, OutVec y) noexcept -> void
{
    ETL_PRECONDITION(A.extent(1) == x.extent(0));
    ETL_PRECONDITION(A.extent(0) == y.extent(0));

    using size_type = detail::common_size_type_t<InMat, InVec, OutVec>;

    for (size_type i{0}; i < A.extent(0); ++i) {
        y[i] = typename OutVec::element_type{};
        for (size_type j{0}; j < A.extent(1); ++j) { y[i] += A[i, j] * x[j]; }
    }
}

}  // namespace etl::linalg
