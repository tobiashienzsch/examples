#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_vector InVec, typename Scalar>
constexpr auto vector_abs_sum(InVec v, Scalar init) noexcept -> Scalar
{
    using std::abs;

    auto sum = init;
    for (typename InVec::size_type i{0}; i < v.extent(0); ++i) { sum += abs(v[i]); }
    return sum;
}

template<detail::in_vector InVec>
constexpr auto vector_abs_sum(InVec v) noexcept -> typename InVec::value_type
{
    return vector_abs_sum(v, typename InVec::value_type{});
}

}  // namespace etl::linalg
