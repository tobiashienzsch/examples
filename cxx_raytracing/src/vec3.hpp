#pragma once

#include <cmath>

namespace mc {

template<typename T>
struct Vec3
{
    T x;
    T y;
    T z;
};

template<typename T>
[[nodiscard]] constexpr auto operator==(Vec3<T> lhs, Vec3<T> rhs) noexcept -> bool;

template<typename T>
[[nodiscard]] constexpr auto operator!=(Vec3<T> lhs, Vec3<T> rhs) noexcept -> bool;

template<typename T>
[[nodiscard]] constexpr auto operator+(Vec3<T> vec) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator-(Vec3<T> vec) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator+(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator-(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator*(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator/(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator*(Vec3<T> lhs, T rhs) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto operator/(Vec3<T> lhs, T rhs) noexcept -> Vec3<T>;

template<typename T>
[[nodiscard]] constexpr auto dot(Vec3<T> lhs, Vec3<T> rhs) noexcept -> T;

template<typename T>
[[nodiscard]] constexpr auto length(Vec3<T> vec) noexcept -> T;

template<typename T>
[[nodiscard]] constexpr auto makeUnit(Vec3<T> vec) noexcept -> Vec3<T>;

}  // namespace mc

namespace mc {
template<typename T>
constexpr auto operator==(Vec3<T> lhs, Vec3<T> rhs) noexcept -> bool
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<typename T>
constexpr auto operator!=(Vec3<T> lhs, Vec3<T> rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

template<typename T>
constexpr auto operator+(Vec3<T> vec) noexcept -> Vec3<T>
{
    return vec;
}

template<typename T>
constexpr auto operator-(Vec3<T> vec) noexcept -> Vec3<T>
{
    return {
        -vec.x,
        -vec.y,
        -vec.z,
    };
}

template<typename T>
constexpr auto operator+(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>
{
    return {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z,
    };
}

template<typename T>
constexpr auto operator-(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>
{
    return {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z,
    };
}

template<typename T>
constexpr auto operator*(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>
{
    return {
        .x = lhs.x * rhs.x,
        .y = lhs.y * rhs.y,
        .z = lhs.z * rhs.z,
    };
}

template<typename T>
constexpr auto operator/(Vec3<T> lhs, Vec3<T> rhs) noexcept -> Vec3<T>
{
    return {
        .x = lhs.x / rhs.x,
        .y = lhs.y / rhs.y,
        .z = lhs.z / rhs.z,
    };
}

template<typename T>
constexpr auto operator*(Vec3<T> lhs, T rhs) noexcept -> Vec3<T>
{
    return {
        .x = lhs.x * rhs,
        .y = lhs.y * rhs,
        .z = lhs.z * rhs,
    };
}

template<typename T>
constexpr auto operator/(Vec3<T> lhs, T rhs) noexcept -> Vec3<T>
{
    return {
        .x = lhs.x / rhs,
        .y = lhs.y / rhs,
        .z = lhs.z / rhs,
    };
}

template<typename T>
constexpr auto dot(Vec3<T> lhs, Vec3<T> rhs) noexcept -> T
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

template<typename T>
constexpr auto length(Vec3<T> vec) noexcept -> T
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

template<typename T>
constexpr auto makeUnit(Vec3<T> vec) noexcept -> Vec3<T>
{
    auto const k = T(1) / length(vec);
    return vec / k;
}

}  // namespace mc
