#pragma once

#include "vec3.hpp"

namespace mc {

template<typename T>
struct Ray
{
    Vec3<T> origin;
    Vec3<T> direction;
};

template<typename T>
[[nodiscard]] constexpr auto pointAt(Ray<T> const& ray, T t) noexcept -> Vec3<T>;

}  // namespace mc

namespace mc {

template<typename T>
constexpr auto pointAt(Ray<T> const& ray, T t) noexcept -> Vec3<T>
{
    return ray.origin + ray.direction * t;
}

}  // namespace mc
