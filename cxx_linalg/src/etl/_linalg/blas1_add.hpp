#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_object InObj1, detail::in_object InObj2, detail::out_object OutObj>
    requires(InObj1::rank() == OutObj::rank() and InObj2::rank() == OutObj::rank())
constexpr auto add(InObj1 x, InObj2 y, OutObj z) -> void
{
    ETL_PRECONDITION(x.extents() == y.extents());

    using size_type = detail::common_size_type_t<InObj1, InObj2, OutObj>;

    if constexpr (OutObj::rank() == 1) {
        for (size_type row{0}; row < x.extent(0); ++row) { z[row] = x[row] + y[row]; }
    } else {
        static_assert(OutObj::rank() == 2);
        for (size_type row{0}; row < x.extent(0); ++row) {
            for (size_type col{0}; col < x.extent(1); ++col) {
                z[row, col] = x[row, col] + y[row, col];
            }
        }
    }
}

}  // namespace etl::linalg
