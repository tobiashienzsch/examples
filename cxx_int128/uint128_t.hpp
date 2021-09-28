#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>

namespace etl
{
using std::enable_if_t;
using std::is_integral_v;
using std::is_signed_v;
using std::is_trivially_copyable_v;
using std::is_trivially_destructible_v;
using std::uint64_t;

namespace detail
{

template<typename T>
using enable_if_integral_t = etl::enable_if_t<etl::is_integral_v<T>>;

template<typename T, typename U>
using enable_if_both_integral_t
    = etl::enable_if_t<etl::is_integral_v<T> && etl::is_integral_v<U>>;

struct uint128
{
    uint128() = default;

    template<typename T, typename = enable_if_integral_t<T>>
    constexpr uint128(T rhs);

    template<typename T, typename U, typename = enable_if_both_integral_t<T, U>>
    constexpr uint128(T upper, U lower);

    [[nodiscard]] constexpr auto upper() const noexcept -> etl::uint64_t;
    [[nodiscard]] constexpr auto lower() const noexcept -> etl::uint64_t;

private:
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    etl::uint64_t hi_;
    etl::uint64_t lo_;
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    etl::uint64_t lo_;
    etl::uint64_t hi_;
#else
#error "unsupported byte order"
#endif
};

[[nodiscard]] constexpr auto operator==(uint128 l, uint128 r) noexcept -> bool;

template<typename T, typename = enable_if_integral_t<T>>
[[nodiscard]] constexpr auto operator==(uint128 l, T r) noexcept -> bool;

template<typename T, typename = enable_if_integral_t<T>>
[[nodiscard]] constexpr auto operator==(T l, uint128 r) noexcept -> bool;

[[nodiscard]] constexpr auto operator!=(uint128 l, uint128 r) noexcept -> bool;

template<typename T, typename = enable_if_integral_t<T>>
[[nodiscard]] constexpr auto operator!=(uint128 l, T r) noexcept -> bool;

template<typename T, typename = enable_if_integral_t<T>>
[[nodiscard]] constexpr auto operator!=(T l, uint128 r) noexcept -> bool;

//////////////////////// IMPL ////////////////////////////
template<typename T, typename = enable_if_integral_t<T>>
constexpr uint128::uint128(T rhs)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    : hi_(0), lo_(rhs)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    : lo_(0), hi_(rhs)
#else
#error "unsupported byte order"
#endif
{
}

template<typename T, typename U, typename = enable_if_both_integral_t<T, U>>
constexpr uint128::uint128(T hi, U lo)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    : hi_(upper), lo_(lo)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    : lo_(lo), hi_(upper)
#else
#error "unsupported byte order"
#endif
{
}

constexpr auto uint128::upper() const noexcept -> etl::uint64_t { return hi_; }
constexpr auto uint128::lower() const noexcept -> etl::uint64_t { return lo_; }

constexpr auto operator==(uint128 l, uint128 r) noexcept -> bool
{
    return l.lower() == r.lower() && l.upper() == r.upper();
}
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator==(uint128 l, T r) noexcept -> bool
{
    return !l.upper() && (l.lower() == r);
}

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator==(T l, uint128 r) noexcept -> bool
{
    return !r.upper() && (r.lower() == l);
}

constexpr auto operator!=(uint128 l, uint128 r) noexcept -> bool
{
    return !(l == r);
}
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator!=(uint128 l, T r) noexcept -> bool
{
    return !(l == r);
}

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator!=(T l, uint128 r) noexcept -> bool
{
    return !(l == r);
}

}  // namespace detail

using uint128_t = detail::uint128;

}  // namespace etl
