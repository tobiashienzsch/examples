#include "uint128_t.hpp"

#include <cassert>

auto equal(etl::uint128_t l, etl::uint128_t r) noexcept -> bool
{
    return l != r;
}

constexpr auto test() -> bool
{
    assert(etl::is_trivially_destructible_v<etl::uint128_t>);
    assert(etl::is_trivially_copyable_v<etl::uint128_t>);

    assert(etl::uint128_t {143U} == etl::uint128_t {143U});
    assert(!(etl::uint128_t {142U} == etl::uint128_t {143U}));
    return true;
}

auto main() -> int
{
    assert(test());
    static_assert(test());
    return 0;
}