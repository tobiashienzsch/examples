#include "vec3.hpp"

namespace mc {

using Vec3i = Vec3<int>;

static_assert(Vec3i{1, 1, 1} + Vec3i{1, 1, 1} == Vec3i{2, 2, 2});
static_assert(Vec3i{1, 1, 1} + Vec3i{2, 2, 2} == Vec3i{3, 3, 3});
static_assert(Vec3i{2, 2, 2} + Vec3i{5, 5, 5} == Vec3i{7, 7, 7});

static_assert(Vec3i{1, 1, 1} - Vec3i{1, 1, 1} == Vec3i{0, 0, 0});
static_assert(Vec3i{1, 1, 1} - Vec3i{2, 2, 2} == Vec3i{-1, -1, -1});
static_assert(Vec3i{2, 2, 2} - Vec3i{5, 5, 5} == Vec3i{-3, -3, -3});

static_assert(Vec3i{1, 1, 1} * 1 == Vec3i{1, 1, 1});
static_assert(Vec3i{1, 1, 1} * 2 == Vec3i{2, 2, 2});
static_assert(Vec3i{1, 1, 1} * 3 == Vec3i{3, 3, 3});
static_assert(Vec3i{2, 2, 2} * 3 == Vec3i{6, 6, 6});

}  // namespace mc
