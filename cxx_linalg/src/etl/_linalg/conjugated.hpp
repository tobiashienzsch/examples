#pragma once

#include <etl/_linalg/accessor_conjugate.hpp>
#include <etl/_linalg/concepts.hpp>

#include <type_traits>

namespace etl::linalg {

template<typename ElementType, typename Extents, typename Layout, typename Accessor>
auto conjugated(mdspan<ElementType, Extents, Layout, Accessor> a)
{
    if constexpr (std::is_arithmetic_v<std::remove_cv_t<ElementType>>) {
        return mdspan<ElementType, Extents, Layout, Accessor>{
            a.data_handle(),
            a.mapping(),
            a.accessor(),
        };
    } else {
        using element_type  = typename accessor_conjugate<Accessor>::element_type;
        using accessor_type = accessor_conjugate<Accessor>;

        return mdspan<element_type, Extents, Layout, accessor_type>{
            a.data_handle(),
            a.mapping(),
            accessor_type(a.accessor()),
        };
    }
}

template<typename ElementType, typename Extents, typename Layout, typename NestedAccessor>
auto conjugated(mdspan<ElementType, Extents, Layout, accessor_conjugate<NestedAccessor>> a)
{
    using element_type  = typename NestedAccessor::element_type;
    using accessor_type = NestedAccessor;

    return mdspan<element_type, Extents, Layout, accessor_type>{
        a.data_handle(),
        a.mapping(),
        a.nested_accessor(),
    };
}

}  // namespace etl::linalg
