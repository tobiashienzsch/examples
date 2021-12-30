#pragma once

#include <cmath>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace etl
{
using std::enable_if_t;
using std::is_integral_v;
using std::is_signed_v;
using std::is_trivially_copyable_v;
using std::is_trivially_destructible_v;
using std::uint64_t;
using std::underlying_type_t;

template<typename... T>
using enable_if_integral_t = enable_if_t<(is_integral_v<T> && ...)>;

struct uint128_t
{
    enum struct upper_t : uint64_t
    {
    };
    enum struct lower_t : uint64_t
    {
    };

    uint128_t() noexcept = default;

    template<typename T, typename = enable_if_integral_t<T>>
    constexpr uint128_t(T val) noexcept;

    constexpr uint128_t(upper_t upper, lower_t lower) noexcept;

    // constexpr uint128_t(std::string_view str) noexcept
    // {
    //     auto val = uint128_t {};
    //     for (int i = 0; str[i] != '\0'; ++i) {
    //         val = val * 10 + str[i] - '0';
    //     }
    //     (*this) = val;
    // }

    constexpr auto upper() const noexcept -> etl::uint64_t;
    constexpr auto lower() const noexcept -> etl::uint64_t;

private:
    etl::uint64_t _parts[2] {};
};

constexpr auto operator==(uint128_t l, uint128_t r) noexcept -> bool;
constexpr auto operator!=(uint128_t l, uint128_t r) noexcept -> bool;

constexpr auto operator^(uint128_t l, uint128_t r) noexcept -> uint128_t;
constexpr auto operator|(uint128_t l, uint128_t r) noexcept -> uint128_t;
constexpr auto operator&(uint128_t l, uint128_t r) noexcept -> uint128_t;
constexpr auto operator~(uint128_t v) noexcept -> uint128_t;

constexpr auto operator+(uint128_t v) noexcept -> uint128_t;
constexpr auto operator+(uint128_t l, uint128_t r) noexcept -> uint128_t;
constexpr auto operator-(uint128_t l, uint128_t r) noexcept -> uint128_t;
constexpr auto operator+=(uint128_t& l, uint128_t r) noexcept -> uint128_t&;
constexpr auto operator-=(uint128_t& l, uint128_t r) noexcept -> uint128_t&;
constexpr auto operator++(uint128_t& val) noexcept -> uint128_t&;
constexpr auto operator--(uint128_t& val) noexcept -> uint128_t&;
constexpr auto operator++(uint128_t& val, int /*ignore*/) noexcept -> uint128_t;
constexpr auto operator--(uint128_t& val, int /*ignore*/) noexcept -> uint128_t;

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator==(uint128_t l, T r) noexcept -> bool;
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator==(T l, uint128_t r) noexcept -> bool;

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator!=(uint128_t l, T r) noexcept -> bool;
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator!=(T l, uint128_t r) noexcept -> bool;

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator^(uint128_t l, T r) noexcept -> uint128_t;
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator^(T l, uint128_t r) noexcept -> uint128_t;

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator|(uint128_t l, T r) noexcept -> uint128_t;
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator|(T l, uint128_t r) noexcept -> uint128_t;

template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator&(uint128_t l, T r) noexcept -> uint128_t;
template<typename T, typename = enable_if_integral_t<T>>
constexpr auto operator&(T l, uint128_t r) noexcept -> uint128_t;

///////////////////////////////////////////////////////////
template<typename T, typename>
inline constexpr uint128_t::uint128_t(T val) noexcept
    : uint128_t {upper_t {0}, lower_t {underlying_type_t<lower_t>(val)}}
{
}

inline constexpr uint128_t::uint128_t(upper_t upper, lower_t lower) noexcept
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    : _parts
{
    underlying_type_t<upper_t>(upper), underlying_type_t<lower_t>(lower)
}
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    : _parts
{
    underlying_type_t<lower_t>(lower), underlying_type_t<upper_t>(upper)
}
#else
#error "unsupported byte order"
#endif
{
}

inline constexpr auto uint128_t::upper() const noexcept -> etl::uint64_t
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return _parts[0];
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return _parts[1];
#else
#error "unsupported byte order"
#endif
}

inline constexpr auto uint128_t::lower() const noexcept -> etl::uint64_t
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return _parts[1];
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return _parts[0];
#else
#error "unsupported byte order"
#endif
}

inline constexpr auto operator==(uint128_t l, uint128_t r) noexcept -> bool
{
    return l.lower() == r.lower() && l.upper() == r.upper();
}

template<typename T, typename>
inline constexpr auto operator==(uint128_t l, T r) noexcept -> bool
{
    return !l.upper() && (l.lower() == r);
}

template<typename T, typename>
inline constexpr auto operator==(T l, uint128_t r) noexcept -> bool
{
    return !r.upper() && (r.lower() == l);
}

inline constexpr auto operator!=(uint128_t l, uint128_t r) noexcept -> bool
{
    return !(l == r);
}

template<typename T, typename>
inline constexpr auto operator!=(uint128_t l, T r) noexcept -> bool
{
    return !(l == r);
}

template<typename T, typename>
inline constexpr auto operator!=(T l, uint128_t r) noexcept -> bool
{
    return !(l == r);
}

inline constexpr auto operator^(uint128_t l, uint128_t r) noexcept -> uint128_t
{
    return {
        uint128_t::upper_t {l.upper() ^ r.upper()},
        uint128_t::lower_t {l.lower() ^ r.lower()},
    };
}

template<typename T, typename>
inline constexpr auto operator^(uint128_t l, T r) noexcept -> uint128_t
{
    return l ^ uint128_t { r };
}

template<typename T, typename>
inline constexpr auto operator^(T l, uint128_t r) noexcept -> uint128_t
{
    return uint128_t {l} ^ r;
}

inline constexpr auto operator|(uint128_t l, uint128_t r) noexcept -> uint128_t
{
    return {
        uint128_t::upper_t {l.upper() | r.upper()},
        uint128_t::lower_t {l.lower() | r.lower()},
    };
}

template<typename T, typename>
inline constexpr auto operator|(uint128_t l, T r) noexcept -> uint128_t
{
    return l | uint128_t {r};
}

template<typename T, typename>
inline constexpr auto operator|(T l, uint128_t r) noexcept -> uint128_t
{
    return uint128_t {l} | r;
}

inline constexpr auto operator&(uint128_t l, uint128_t r) noexcept -> uint128_t
{
    return {
        uint128_t::upper_t {l.upper() & r.upper()},
        uint128_t::lower_t {l.lower() & r.lower()},
    };
}

template<typename T, typename>
inline constexpr auto operator&(uint128_t l, T r) noexcept -> uint128_t
{
    return l & uint128_t {r};
}

template<typename T, typename>
inline constexpr auto operator&(T l, uint128_t r) noexcept -> uint128_t
{
    return uint128_t {l} & r;
}

inline constexpr auto operator+(uint128_t v) noexcept -> uint128_t { return v; }

inline constexpr auto operator~(uint128_t v) noexcept -> uint128_t
{
    return {
        uint128_t::upper_t {~v.upper()},
        uint128_t::lower_t {~v.lower()},
    };
}

inline constexpr auto operator+(uint128_t l, uint128_t r) noexcept -> uint128_t
{
    auto upper = l.upper() + ((l.lower() + r.lower()) < l.lower());
    auto lower = l.lower() + r.lower();
    return {
        uint128_t::upper_t {upper},
        uint128_t::lower_t {lower},
    };
}

inline constexpr auto operator+=(uint128_t& l, uint128_t r) noexcept
    -> uint128_t&
{
    l = l + r;
    return l;
}

inline constexpr auto operator-(uint128_t l, uint128_t r) noexcept -> uint128_t
{
    auto upper = l.upper() - r.upper() - ((l.lower() - r.lower()) > l.lower());
    auto lower = l.lower() - r.lower();
    return {
        uint128_t::upper_t {upper},
        uint128_t::lower_t {lower},
    };
}

inline constexpr auto operator-=(uint128_t& l, uint128_t r) noexcept
    -> uint128_t&
{
    l = l - r;
    return l;
}

inline constexpr auto operator++(uint128_t& val) noexcept -> uint128_t&
{
    val += 1;
    return val;
}

inline constexpr auto operator--(uint128_t& val) noexcept -> uint128_t&
{
    val -= 1;
    return val;
}

inline constexpr auto operator++(uint128_t& val, int /*ignore*/) noexcept
    -> uint128_t
{
    val += 1;
    return val;
}

inline constexpr auto operator--(uint128_t& val, int /*ignore*/) noexcept
    -> uint128_t
{
    val -= 1;
    return val;
}

}  // namespace etl