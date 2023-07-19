#pragma once

#include <etl/mdspan.hpp>

#include <complex>
#include <concepts>
#include <type_traits>

namespace etl::linalg::detail {

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

}  // namespace etl::linalg::detail
