#pragma once

#include <etl/_linalg/scaled_scalar.hpp>

#include <type_traits>

namespace etl::linalg {

template<typename ScalingFactor, typename Accessor>
    requires(std::is_copy_constructible_v<typename Accessor::reference>)
struct accessor_scaled
{
    using reference = detail::scaled_scalar<
        ScalingFactor,
        typename Accessor::reference,
        typename Accessor::element_type>;
    using element_type     = std::add_const_t<typename reference::value_type>;
    using data_handle_type = Accessor::data_handle_type;
    using offset_policy    = accessor_scaled<ScalingFactor, typename Accessor::offset_policy>;

    constexpr accessor_scaled(ScalingFactor const& s, Accessor const& a)
        : _scaling_factor{s}
        , _nested_accessor{a}
    {}

    [[nodiscard]] constexpr auto access(data_handle_type p, size_t i) const noexcept
        -> reference
    {
        return reference(_scaling_factor, _nested_accessor.access(p, i));
    }

    [[nodiscard]] constexpr auto offset(data_handle_type p, size_t i) const noexcept
        -> offset_policy::data_handle_type
    {
        return _nested_accessor.offset(p, i);
    }

    [[nodiscard]] constexpr auto scaling_factor() const -> ScalingFactor
    {
        return _scaling_factor;
    }

    [[nodiscard]] constexpr auto nested_accessor() const -> Accessor
    {
        return _nested_accessor;
    }

private:
    ScalingFactor _scaling_factor;
    Accessor _nested_accessor;
};

}  // namespace etl::linalg
