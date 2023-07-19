#pragma once

#include <etl/mdspan.hpp>

#include <cmath>
#include <complex>
#include <concepts>
#include <cstdlib>
#include <type_traits>

namespace etl::linalg::detail {

template<typename T>
inline constexpr auto always_false = false;

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

namespace abs_if_needed_detail {

using std::abs;

template<typename T>
auto abs(T const&) -> T = delete;

template<typename T>
concept has_abs = requires { abs(std::declval<T>()); };

}  // namespace abs_if_needed_detail

inline constexpr auto abs_if_needed = []<typename T>(T const& val) {
    if constexpr (std::unsigned_integral<T>) {
        return val;
    } else if constexpr (abs_if_needed_detail::has_abs<T>) {
        using std::abs;
        return abs(val);
    } else {
        static_assert(always_false<T>);
    }
};

namespace conj_if_needed_detail {

using std::conj;

template<typename T>
auto conj(T const&) -> T = delete;

template<typename T>
concept has_conj = requires { conj(std::declval<T>()); };

}  // namespace conj_if_needed_detail

inline constexpr auto conj_if_needed = []<typename T>(T const& val) {
    if constexpr (std::is_arithmetic_v<T>) {
        return val;
    } else if constexpr (conj_if_needed_detail::has_conj<T>) {
        using std::conj;
        return conj(val);
    } else {
        return val;
    }
};

namespace real_if_needed_detail {

using std::real;

template<typename T>
auto real(T const&) -> T = delete;

template<typename T>
concept has_real = requires { real(std::declval<T>()); };

}  // namespace real_if_needed_detail

inline constexpr auto real_if_needed = []<typename T>(T const& val) {
    if constexpr (std::is_arithmetic_v<T>) {
        return val;
    } else if constexpr (real_if_needed_detail::has_real<T>) {
        using std::real;
        return real(val);
    } else {
        return val;
    }
};

namespace imag_if_needed_detail {

using std::imag;

template<typename T>
auto imag(T const&) -> T = delete;

template<typename T>
concept has_imag = requires { imag(std::declval<T>()); };

}  // namespace imag_if_needed_detail

inline constexpr auto imag_if_needed = []<typename T>(T const& val) {
    if constexpr (std::is_arithmetic_v<T>) {
        return val;
    } else if constexpr (imag_if_needed_detail::has_imag<T>) {
        using std::imag;
        return imag(val);
    } else {
        return T{};
    }
};

}  // namespace etl::linalg::detail
