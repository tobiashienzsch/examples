#pragma once

template<typename T>
struct Vec3
{
    constexpr explicit(false) Vec3() noexcept = default;

    constexpr explicit(true) Vec3(T val) noexcept
        : x {val}, y {val}, z {val} { }

    constexpr explicit(false) Vec3(T x, T y, T z) noexcept : x {x}, y {y}, z {z}
    {
    }

    T x {0};
    T y {0};
    T z {0};
};

template<typename T>
[[nodiscard]] constexpr auto operator==(Vec3<T> lhs, Vec3<T> rhs) -> bool
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<typename T>
[[nodiscard]] constexpr auto operator!=(Vec3<T> lhs, Vec3<T> rhs) -> bool
{
    return !(lhs == rhs);
}

template<typename T>
[[nodiscard]] constexpr auto operator+(Vec3<T> lhs, Vec3<T> rhs) -> Vec3<T>
{
    return Vec3<T> {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
    };
}

template<typename T>
constexpr auto operator+=(Vec3<T>& lhs, Vec3<T> rhs) -> Vec3<T>&
{
    lhs = lhs + rhs;
    return lhs;
}

template<typename T>
[[nodiscard]] constexpr auto operator-(Vec3<T> lhs, Vec3<T> rhs) -> Vec3<T>
{
    return Vec3<T> {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
    };
}

template<typename T>
constexpr auto operator-=(Vec3<T>& lhs, Vec3<T> rhs) -> Vec3<T>&
{
    lhs = lhs - rhs;
    return lhs;
}

template<typename T>
[[nodiscard]] constexpr auto operator*(Vec3<T> lhs, Vec3<T> rhs) -> Vec3<T>
{
    return Vec3<T> {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
}

template<typename T>
[[nodiscard]] constexpr auto operator*(Vec3<T> lhs, T rhs) -> Vec3<T>
{
    return Vec3<T> {
        lhs.x * rhs,
        lhs.y * rhs,
        lhs.z * rhs,
    };
}

template<typename T>
[[nodiscard]] constexpr auto operator*(T lhs, Vec3<T> rhs) -> Vec3<T>
{
    return Vec3<T> {
        lhs * rhs.x,
        lhs * rhs.y,
        lhs * rhs.z,
    };
}

template<typename T>
constexpr auto operator*=(Vec3<T>& lhs, Vec3<T> rhs) -> Vec3<T>&
{
    lhs = lhs * rhs;
    return lhs;
}

template<typename T>
constexpr auto operator*=(Vec3<T>& lhs, T rhs) -> Vec3<T>&
{
    lhs = lhs * rhs;
    return lhs;
}

template<typename T>
[[nodiscard]] constexpr auto operator/(Vec3<T> lhs, T rhs) -> Vec3<T>
{
    return Vec3<T> {
        lhs.x / rhs,
        lhs.y / rhs,
        lhs.z / rhs,
    };
}

template<typename T>
constexpr auto operator/=(Vec3<T>& lhs, T rhs) -> Vec3<T>&
{
    lhs = lhs / rhs;
    return lhs;
}
