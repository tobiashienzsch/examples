#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace etl
{
namespace detail
{

template<size_t Index, typename T>
struct derive_element : T
{
    using element_type = T;
    using T::T;

    T& elem() & { return *this; }
    T&& elem() && { return std::move(*this); }
    T const& elem() const& { return *this; }
    T const&& elem() const&& { return std::move(*this); }
};

template<size_t Index, typename T>
struct wrap_element
{
    using element_type = T;

    template<typename... Args>
    wrap_element(Args&&... args) : value(std::forward<Args>(args)...)
    {
    }

    T& elem() & { return *this; }
    T&& elem() && { return std::move(*this); }
    T const& elem() const& { return *this; }
    T const&& elem() const&& { return std::move(*this); }

    T value;
};

template<size_t Index, typename T>
using wrap = std::conditional_t<std::is_empty_v<T> && !std::is_final_v<T> && !std::is_union_v<T>,
                                derive_element<Index, T>, wrap_element<Index, T>>;

template<typename...>
struct tuple_base;

template<size_t... Indices, typename... Types>
struct tuple_base<std::index_sequence<Indices...>, Types...> : wrap<Indices, Types>...
{
    //   using wrap<Indices, Types>::wrap...;
};
}  // namespace detail

template<typename... Types>
struct tuple : detail::tuple_base<std::index_sequence_for<Types...>, Types...>
{
    using base = detail::tuple_base<std::index_sequence_for<Types...>, Types...>;
    using base::base;
};
}  // namespace etl
