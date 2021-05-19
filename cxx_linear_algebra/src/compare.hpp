#pragma once

#include <cmath>
#include <limits>

namespace math
{
template<typename T>
[[nodiscard]] auto closeEnough(T a, T b) -> bool
{
    return std::abs(a - b) < (std::numeric_limits<T>::epsilon() * T {16});
}

}  // namespace math
