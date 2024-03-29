#pragma once

#include <etl/_linalg/accessor_scaled.hpp>
#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<
    typename ScalingFactor,
    typename ElementType,
    typename Extents,
    typename Layout,
    typename Accessor>
[[nodiscard]] constexpr auto
scaled(ScalingFactor alpha, mdspan<ElementType, Extents, Layout, Accessor> x)
{
    return mdspan{
        x.data_handle(),
        x.mapping(),
        accessor_scaled{alpha, x.accessor()}
    };
}

}  // namespace etl::linalg
