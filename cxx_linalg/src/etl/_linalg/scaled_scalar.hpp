#pragma once

#include <etl/_linalg/proxy_reference.hpp>

#include <type_traits>

namespace etl::linalg::detail {

template<typename ScalingFactor, typename ReferenceValue>
concept scalable
    = requires { std::declval<ScalingFactor>() * std::declval<ReferenceValue>(); };

template<typename ScalingFactor, typename Reference, typename ReferenceValue>
    requires(scalable<ScalingFactor, ReferenceValue>)
struct scaled_scalar
    : proxy_reference<
          Reference,
          ReferenceValue,
          scaled_scalar<ScalingFactor, Reference, ReferenceValue>>
{
    using value_type
        = decltype(std::declval<ScalingFactor>() * ReferenceValue(std::declval<Reference>()));

    explicit scaled_scalar(ScalingFactor const& scaling_factor, Reference const& reference)
        : proxy_reference<
            Reference,
            ReferenceValue,
            scaled_scalar<ScalingFactor, Reference, ReferenceValue>>{reference}
        , _scaling_factor{scaling_factor}
    {}

    [[nodiscard]] auto to_value(Reference reference) const -> value_type
    {
        return _scaling_factor * ReferenceValue(reference);
    }

private:
    ScalingFactor _scaling_factor;
};

}  // namespace etl::linalg::detail
