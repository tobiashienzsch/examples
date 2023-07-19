#pragma once

#include <etl/_linalg/concepts.hpp>
#include <etl/_linalg/contracts.hpp>

namespace etl::linalg {

template<detail::in_matrix InMat, detail::in_vector InVec, detail::out_vector OutVec>
constexpr auto matrix_vector_product(InMat A, InVec x, OutVec y) noexcept -> void
{
    ETL_PRECONDITION(A.extent(1) == x.extent(0));
    ETL_PRECONDITION(A.extent(0) == y.extent(0));

    using size_type = detail::common_size_type_t<InMat, InVec, OutVec>;

    for (size_type i{0}; i < A.extent(0); ++i) {
        y[i] = typename OutVec::element_type{};
        for (size_type j{0}; j < A.extent(1); ++j) { y[i] += A[i, j] * x[j]; }
    }
}

}  // namespace etl::linalg
