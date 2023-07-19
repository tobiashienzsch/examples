#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_matrix InMat, typename Scalar>
constexpr auto matrix_frob_norm(InMat A, Scalar init) noexcept -> Scalar
{
    using std::abs;
    using std::sqrt;

    auto result = init;
    for (typename InMat::size_type row{0}; row < A.extent(0); ++row) {
        for (typename InMat::size_type col{0}; col < A.extent(1); ++col) {
            result += abs(A[row, col]);
        }
    }
    return sqrt(result);
}

}  // namespace etl::linalg
