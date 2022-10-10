#pragma once

#include "vec3.hpp"

namespace mc
{

template<typename T>
struct Sphere
{
    Vec3<T> center;
    T radius;
};

}  // namespace mc
