#pragma once

#include "matrix.hpp"
#include "vector.hpp"

namespace math
{

/// INTERFACE
/////////////////////////////////////////////////////////////////////////
template<typename T>
[[nodiscard]] auto linearSolve(Matrix<T> const& A, Vector<T> const& b)
    -> Vector<T>;

/// IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////
template<typename T>
auto linearSolve(Matrix<T> const& A, Vector<T> const& b) -> Vector<T>
{
    using ssize_type = std::make_signed_t<decltype(A.size())>;

    auto bMat = Matrix<T> {b.size(), 1};
    for (decltype(b.size()) row {0}; row < b.size(); ++row)
    {
        bMat(row, 0) = b[row];
    }

    auto rowEchelonMat = join(A, bMat);
    rowEchelon(rowEchelonMat);

    auto result         = Vector<T> {b.size()};
    auto const numRows  = static_cast<ssize_type>(rowEchelonMat.rows());
    auto const numCols  = static_cast<ssize_type>(rowEchelonMat.cols());
    auto const startRow = static_cast<ssize_type>(numRows - 1);

    for (int i = startRow; i >= 0; --i)
    {
        auto const currentResult = rowEchelonMat(i, numCols - 1);

        auto sum = T {};
        for (auto j = i + 1; j < numRows; ++j)
        {
            sum += (rowEchelonMat(i, j) * result[j]);
        }

        result[i] = (currentResult - sum) / rowEchelonMat(i, i);
    }

    return result;
}

}  // namespace math
