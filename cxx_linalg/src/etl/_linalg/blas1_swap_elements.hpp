#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

#include <utility>

namespace etl::linalg {

template<detail::inout_object InOutObj1, detail::inout_object InOutObj2>
    requires(InOutObj1::rank() == InOutObj1::rank())
constexpr auto swap_elements(InOutObj1 x, InOutObj2 y) -> void
{
    ETL_PRECONDITION(x.extents() == y.extents());

    using std::swap;
    using size_type = detail::common_size_type_t<InOutObj1, InOutObj2>;

    if constexpr (InOutObj1::rank() == 1) {
        for (size_type i{0}; i < x.extent(0); ++i) { swap(x[i], y[i]); }
    } else {
        static_assert(InOutObj1::rank() == 2);
        for (size_type i{0}; i < x.extent(0); ++i) {
            for (size_type j{0}; j < x.extent(1); ++j) { swap(x[i, j], y[i, j]); }
        }
    }
}

}  // namespace etl::linalg
