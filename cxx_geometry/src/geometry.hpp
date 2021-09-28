#pragma once

#include <cmath>
#include <iostream>
#include <numbers>
#include <numeric>
#include <optional>

namespace mc
{
template<typename T>
struct Point
{
    Point(T initialX, T initialY) : x {initialX}, y {initialY} { }
    T x;
    T y;
};

template<typename T>
[[nodiscard]] constexpr auto operator==(Point<T> lhs, Point<T> rhs) noexcept
    -> bool
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

template<typename T>
[[nodiscard]] constexpr auto operator!=(Point<T> lhs, Point<T> rhs) noexcept
    -> bool
{
    return !(lhs == rhs);
}

template<typename T>
[[nodiscard]] constexpr auto operator<<(std::ostream& out, Point<T> const point)
    -> std::ostream&
{
    out << "Point(" << point.x << ", " << point.y << ')';
    return out;
}

template<typename T>
[[nodiscard]] constexpr auto isOrigin(Point<T> const point) noexcept -> bool
{
    return (point.x == T {}) && (point.y == T {});
}

template<typename T>
struct Line
{
    Line(Point<T> const initialStart, Point<T> const initialEnd)
        : start {initialStart}, end {initialEnd}
    {
    }
    Point<T> start;
    Point<T> end;
};

template<typename T>
[[nodiscard]] constexpr auto operator<<(std::ostream& out, Line<T> const line)
    -> std::ostream&
{
    out << "Line(" << line.start << ", " << line.end << ')';
    return out;
}

template<typename T>
[[nodiscard]] constexpr auto isEmpty(Line<T> const line) noexcept -> bool
{
    return line.start == line.end;
}

template<typename T>
[[nodiscard]] constexpr auto slope(Line<T> const line) noexcept
    -> std::optional<T>
{
    auto const x = line.end.x - line.start.x;
    auto const y = line.end.y - line.start.y;
    if (x == T {})
    {
        return std::nullopt;
    }
    return (y / x);
}
template<typename T>
[[nodiscard]] constexpr auto midpoint(Line<T> l) noexcept -> Point<T>
{
    return {
        std::midpoint(l.start.x, l.end.x),
        std::midpoint(l.start.y, l.end.y),
    };
}

template<typename T>
[[nodiscard]] constexpr auto angleInRadians(Line<T> l) noexcept -> T
{
    auto const angle = std::atan2(l.end.y - l.start.y, l.end.x - l.start.x);
    return static_cast<T>(angle);
}

template<typename T>
[[nodiscard]] constexpr auto angleInDegrees(Line<T> l) noexcept -> T
{
    return angleInRadians(l) * (T {180} / std::numbers::pi_v<T>);
}

auto slopef(Line<float> line) -> float { return slope(line).value_or(0.0f); }
auto midpointf(Line<float> line) -> Point<float> { return midpoint(line); }
}  // namespace mc