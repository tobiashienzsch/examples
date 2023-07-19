#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

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

}  // namespace etl::linalg
