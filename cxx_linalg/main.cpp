#include <etl/linalg.hpp>
#include <etl/mdspan.hpp>

using value_t  = float;
using vector_t = etl::mdspan<value_t, etl::dextents<int, 1>>;
using matrix_t = etl::mdspan<value_t, etl::dextents<int, 2>>;

[[maybe_unused]] static auto vector_two_norm(vector_t v, value_t init) noexcept -> value_t
{
    return etl::linalg::vector_two_norm(v, init);
}

[[maybe_unused]] static auto vector_abs_sum(vector_t v, value_t init) noexcept -> value_t
{
    return etl::linalg::vector_abs_sum(v, init);
}

[[maybe_unused]] static auto swap_elements(matrix_t lhs, matrix_t rhs) noexcept -> void
{
    etl::linalg::swap_elements(lhs, rhs);
}

[[maybe_unused]] static auto scale(matrix_t v, value_t alpha) noexcept -> void
{
    etl::linalg::scale(alpha, v);
}

[[maybe_unused]] static auto matrix_frob_norm(matrix_t v) noexcept -> value_t
{
    return etl::linalg::matrix_frob_norm(v, value_t{});
}

[[maybe_unused]] static auto
matrix_vector_product(matrix_t A, vector_t x, vector_t y) noexcept -> void
{
    etl::linalg::matrix_vector_product(A, x, y);
}

auto main() -> int
{
    auto matrix = etl::experimental::mdarray<value_t, etl::dextents<int, 2>>{16, 16};
    auto vector = etl::experimental::mdarray<value_t, etl::dextents<int, 1>>{16};
    auto out    = etl::experimental::mdarray<value_t, etl::dextents<int, 1>>{16};
    etl::linalg::matrix_vector_product(
        matrix.to_mdspan(),
        vector.to_mdspan(),
        out.to_mdspan()
    );

    return 0;
}