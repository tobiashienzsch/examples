#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_object InObj, detail::out_object OutObj>
    requires(InObj::rank() == OutObj::rank())
constexpr auto copy(InObj x, OutObj y) -> void
{
    ETL_PRECONDITION(x.extents() == y.extents());

    using size_type = detail::common_size_type_t<InObj, OutObj>;

    if constexpr (InObj::rank() == 1) {
        for (size_type i{0}; i < x.extent(0); ++i) { y[i] = x[i]; }
    } else {
        static_assert(InObj::rank() == 2);
        for (size_type i{0}; i < x.extent(0); ++i) {
            for (size_type j{0}; j < x.extent(1); ++j) { y[i, j] = x[i, j]; }
        }
    }
}

}  // namespace etl::linalg
