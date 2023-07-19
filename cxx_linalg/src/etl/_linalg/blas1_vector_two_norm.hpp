#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

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

}  // namespace etl::linalg
