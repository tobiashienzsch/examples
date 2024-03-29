#pragma once

#include <etl/_linalg/concepts.hpp>

#include <type_traits>

namespace etl::linalg::detail {

struct proxy_reference_base
{};

template<typename Reference, typename Value, typename Derived>
struct proxy_reference : proxy_reference_base
{
    using reference_type = Reference;
    using value_type     = Value;
    using derived_type   = Derived;

    constexpr explicit proxy_reference(Reference reference) : _reference(reference) {}

    constexpr operator value_type() const
    {
        return static_cast<Derived const&>(*this).to_value(_reference);
    }

    constexpr friend auto operator-(derived_type const& cs) { return -value_type(cs); }

    template<typename Rhs>
        requires(std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator+(derived_type lhs, Rhs rhs)
    {
        using rhs_value_type = typename Rhs::value_type;
        return value_type(lhs) + rhs_value_type(rhs);
    }

    template<typename Rhs>
        requires(not std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator+(derived_type lhs, Rhs rhs)
    {
        return value_type(lhs) + rhs;
    }

    template<typename Lhs>
        requires(not std::is_base_of_v<proxy_reference_base, Lhs>)
    constexpr friend auto operator+(Lhs lhs, derived_type rhs)
    {
        return lhs + value_type(rhs);
    }

    template<typename Rhs>
        requires(std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator-(derived_type lhs, Rhs rhs)
    {
        using rhs_value_type = typename Rhs::value_type;
        return value_type(lhs) - rhs_value_type(rhs);
    }

    template<typename Rhs>
        requires(not std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator-(derived_type lhs, Rhs rhs)
    {
        return value_type(lhs) - rhs;
    }

    template<typename Lhs>
        requires(not std::is_base_of_v<proxy_reference_base, Lhs>)
    constexpr friend auto operator-(Lhs lhs, derived_type rhs)
    {
        return lhs - value_type(rhs);
    }

    template<typename Rhs>
        requires(std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator*(derived_type lhs, Rhs rhs)
    {
        using rhs_value_type = typename Rhs::value_type;
        return value_type(lhs) * rhs_value_type(rhs);
    }

    template<typename Rhs>
        requires(not std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator*(derived_type lhs, Rhs rhs)
    {
        return value_type(lhs) * rhs;
    }

    template<typename Lhs>
        requires(not std::is_base_of_v<proxy_reference_base, Lhs>)
    constexpr friend auto operator*(Lhs lhs, derived_type rhs)
    {
        return lhs * value_type(rhs);
    }

    template<typename Rhs>
        requires(std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator/(derived_type lhs, Rhs rhs)
    {
        using rhs_value_type = typename Rhs::value_type;
        return value_type(lhs) / rhs_value_type(rhs);
    }

    template<typename Rhs>
        requires(not std::is_base_of_v<proxy_reference_base, Rhs>)
    constexpr friend auto operator/(derived_type lhs, Rhs rhs)
    {
        return value_type(lhs) / rhs;
    }

    template<typename Lhs>
        requires(not std::is_base_of_v<proxy_reference_base, Lhs>)
    constexpr friend auto operator/(Lhs lhs, derived_type rhs)
    {
        return lhs / value_type(rhs);
    }

    constexpr friend auto abs(derived_type const& x)
    {
        return abs_if_needed(value_type(static_cast<this_type const&>(x)));
    }

    constexpr friend auto real(derived_type const& x)
    {
        return real_if_needed(value_type(static_cast<this_type const&>(x)));
    }

    constexpr friend auto imag(derived_type const& x)
    {
        return imag_if_needed(value_type(static_cast<this_type const&>(x)));
    }

    constexpr friend auto conj(derived_type const& x)
    {
        return conj_if_needed(value_type(static_cast<this_type const&>(x)));
    }

private:
    using this_type = proxy_reference<Reference, Value, Derived>;

    Reference _reference;
};

}  // namespace etl::linalg::detail
