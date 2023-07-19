#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_vector InVec, typename Scalar>
constexpr auto vector_abs_sum(InVec v, Scalar init) noexcept -> Scalar
{
    auto sum = init;
    for (typename InVec::size_type i{0}; i < v.extent(0); ++i) {
        if constexpr (std::is_arithmetic_v<typename InVec::value_type>) {
            sum += detail::abs_if_needed(v[i]);
        } else {
            sum += detail::abs_if_needed(detail::real_if_needed(v[i]));
            sum += detail::abs_if_needed(detail::imag_if_needed(v[i]));
        }
    }
    return sum;
}

template<detail::in_vector InVec>
constexpr auto vector_abs_sum(InVec v) noexcept -> typename InVec::value_type
{
    return vector_abs_sum(v, typename InVec::value_type{});
}

}  // namespace etl::linalg
