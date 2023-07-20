#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/proxy_reference.hpp>

#include <concepts>
#include <type_traits>

namespace etl::linalg::detail {

template<typename ReferenceValue>
concept conjugatable = requires {
    {
        conj_if_needed(std::declval<ReferenceValue>())
    } -> std::same_as<ReferenceValue>;
};

template<typename Reference, conjugatable ReferenceValue>
struct conjugated_scalar
    : proxy_reference<
          Reference,
          ReferenceValue,
          conjugated_scalar<Reference, ReferenceValue>>
{
private:
    using my_type   = conjugated_scalar<Reference, ReferenceValue>;
    using base_type = proxy_reference<Reference, ReferenceValue, my_type>;

public:
    using value_type = decltype(conj_if_needed(ReferenceValue(std::declval<Reference>())));

    constexpr explicit conjugated_scalar(Reference reference) : base_type{reference} {}

    [[nodiscard]] static constexpr auto to_value(Reference reference)
    {
        return conj_if_needed(ReferenceValue(reference));
    }
};

}  // namespace etl::linalg::detail
