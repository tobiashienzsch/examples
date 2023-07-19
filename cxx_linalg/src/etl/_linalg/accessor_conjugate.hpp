#pragma once

#include <etl/_linalg/conjugated_scalar.hpp>

#include <type_traits>

namespace etl::linalg {

template<typename Accessor>
struct accessor_conjugate
{
    using reference = std::conditional_t<
        std::is_arithmetic_v<std::remove_cv_t<typename Accessor::element_type>>,
        typename Accessor::reference,
        detail::conjugated_scalar<
            typename Accessor::reference,
            std::remove_cv_t<typename Accessor::element_type>>>;
    using element_type     = std::add_const_t<std::conditional_t<
        std::is_arithmetic_v<std::remove_cv_t<typename Accessor::element_type>>,
        typename Accessor::element_type,
        typename reference::value_type>>;
    using data_handle_type = typename Accessor::data_handle_type;
    using offset_policy    = std::conditional_t<
        std::is_arithmetic_v<std::remove_cv_t<typename Accessor::element_type>>,
        typename Accessor::offset_policy,
        accessor_conjugate<typename Accessor::offset_policy>>;

    constexpr accessor_conjugate(Accessor a) : _nested_accessor(a) {}

    [[nodiscard]] constexpr auto access(data_handle_type p, size_t i) const
        noexcept(noexcept(reference(_nested_accessor.access(p, i)))) -> reference
    {
        return reference(_nested_accessor.access(p, i));
    }

    [[nodiscard]] constexpr auto offset(data_handle_type p, size_t i) const
        noexcept(noexcept(_nested_accessor.offset(p, i))) ->
        typename offset_policy::data_handle_type
    {
        _nested_accessor.offset(p, i);
    }

    [[nodiscard]] constexpr auto nested_accessor() const -> Accessor
    {
        return _nested_accessor;
    }

private:
    Accessor _nested_accessor;
};

}  // namespace etl::linalg
