template<typename T>
struct vec3
{
    T x;
    T y;
    T z;
};

using vec3i = vec3<int>;
using vec3f = vec3<float>;

template<typename T>
[[nodiscard]] constexpr auto operator==(vec3<T> lhs, vec3<T> rhs) -> bool
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<typename T>
[[nodiscard]] constexpr auto operator!=(vec3<T> lhs, vec3<T> rhs) -> bool
{
    return !(lhs == rhs);
}

template<typename T>
[[nodiscard]] constexpr auto operator+(vec3<T> lhs, vec3<T> rhs) -> vec3<T>
{
    return {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z,
    };
}

static_assert(vec3i {1, 1, 1} + vec3i {1, 1, 1} == vec3i {2, 2, 2});
static_assert(vec3i {1, 1, 1} + vec3i {2, 2, 2} == vec3i {3, 3, 3});
static_assert(vec3i {2, 2, 2} + vec3i {5, 5, 5} == vec3i {7, 7, 7});

template<typename T>
[[nodiscard]] constexpr auto operator-(vec3<T> lhs, vec3<T> rhs) -> vec3<T>
{
    return {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z,
    };
}

static_assert(vec3i {1, 1, 1} - vec3i {1, 1, 1} == vec3i {0, 0, 0});
static_assert(vec3i {1, 1, 1} - vec3i {2, 2, 2} == vec3i {-1, -1, -1});
static_assert(vec3i {2, 2, 2} - vec3i {5, 5, 5} == vec3i {-3, -3, -3});

template<typename T>
[[nodiscard]] constexpr auto operator*(vec3<T> lhs, vec3<T> rhs) -> vec3<T>
{
    return {
        .x = lhs.y * rhs.z - lhs.z * rhs.y,
        .y = lhs.z * rhs.x - lhs.x * rhs.z,
        .z = lhs.x * rhs.y - lhs.y * rhs.x,
    };
}

template<typename T>
[[nodiscard]] constexpr auto operator*(vec3<T> lhs, T rhs) -> vec3<T>
{
    return {
        .x = lhs.x * rhs,
        .y = lhs.y * rhs,
        .z = lhs.z * rhs,
    };
}

static_assert(vec3i {1, 1, 1} * 1 == vec3i {1, 1, 1});
static_assert(vec3i {1, 1, 1} * 2 == vec3i {2, 2, 2});
static_assert(vec3i {1, 1, 1} * 3 == vec3i {3, 3, 3});
static_assert(vec3i {2, 2, 2} * 3 == vec3i {6, 6, 6});

template<typename T>
struct mat33
{
    T data[3][3];  // NOLINT
};

namespace detail
{

template<typename MatrixT, unsigned Size>
auto matrix_mul(MatrixT l, MatrixT r) -> MatrixT
{
    MatrixT res {};
    for (auto i = 0U; i < Size; ++i)
    {
        for (auto j = 0U; j < Size; ++j)
        {
            for (auto k = 0U; k < Size; ++k)
            {
                res.data[i][j] += l.data[i][k] * r.data[k][j];
            }
        }
    }

    return res;
}
}  // namespace detail

template<typename T>
[[nodiscard]] constexpr auto operator*(mat33<T> lhs, mat33<T> rhs) -> mat33<T>
{
    return detail::matrix_mul<mat33<T>, 3>(lhs, rhs);
}

auto main() -> int
{
    auto m1 = mat33<double> {};
    auto m2 = mat33<double> {};
    auto m3 = m1 * m2;
    return static_cast<int>(m3.data[0][0]);
}