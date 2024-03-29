#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<typename Scalar, detail::inout_object InOutObj>
constexpr auto scale(Scalar alpha, InOutObj x) -> void
{
    using size_type = typename InOutObj::size_type;

    if constexpr (InOutObj::rank() == 1) {
        for (size_type i{0}; i < x.extent(0); ++i) { x[i] = x[i] * alpha; }
    } else {
        static_assert(InOutObj::rank() == 2);
        for (size_type i{0}; i < x.extent(0); ++i) {
            for (size_type j{0}; j < x.extent(1); ++j) { x[i, j] = x[i, j] * alpha; }
        }
    }
}

}  // namespace etl::linalg
