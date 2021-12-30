#include "uint128_t.hpp"

#include <cassert>

constexpr auto test() -> bool
{
    using etl::uint128_t;

    assert(etl::is_trivially_destructible_v<uint128_t>);
    assert(etl::is_trivially_copyable_v<uint128_t>);

    assert(uint128_t {0} == 0U);
    assert(uint128_t {1} == 1U);
    assert(uint128_t {2} == uint128_t {2});
    assert(uint128_t {1} == uint128_t {1} + uint128_t {0});
    assert(uint128_t {3} == uint128_t {1} + uint128_t {2});
    assert(uint128_t {1} == uint128_t {1} - uint128_t {0});
    assert(uint128_t {1} == uint128_t {2} - uint128_t {1});
    return true;
}

auto main() -> int
{
    assert(test());
    static_assert(test());
    return 0;
}