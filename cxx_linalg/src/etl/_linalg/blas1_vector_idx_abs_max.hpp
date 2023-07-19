#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_vector InVec>
constexpr auto idx_abs_max(InVec v) -> typename InVec::size_type
{
    auto get_value = [](auto val) {
        if constexpr (std::is_arithmetic_v<typename InVec::value_type>) {
            return detail::abs_if_needed(val);
        } else {
            auto const re = detail::abs_if_needed(detail::real_if_needed(val));
            auto const im = detail::abs_if_needed(detail::imag_if_needed(val));
            return re + im;
        }
    };

    auto idx   = std::numeric_limits<typename InVec::size_type>::max();
    auto max_v = std::numeric_limits<typename InVec::value_type>::min();

    for (typename InVec::size_type i{0}; i < v.extent(0); ++i) {
        if (auto const val = get_value(v[i]); val > max_v) {
            idx   = i;
            max_v = val;
        }
    }

    return idx;
}

}  // namespace etl::linalg
