#pragma once

#include "vector.hpp"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace math
{

template<typename T>
struct Matrix
{
    using value_type = T;
    using size_type  = std::uint32_t;

    Matrix() noexcept = default;
    Matrix(size_type rows, size_type cols);
    Matrix(Matrix<T> const& other);

    auto operator=(Matrix<T> const& other) -> Matrix<T>&;

    auto clear() noexcept -> void;
    auto resize(size_type rows, size_type cols) -> void;

    [[nodiscard]] auto size() const noexcept -> size_type;
    [[nodiscard]] auto rows() const noexcept -> size_type;
    [[nodiscard]] auto cols() const noexcept -> size_type;

    [[nodiscard]] auto at(size_type row, size_type col) -> value_type&;
    [[nodiscard]] auto at(size_type row, size_type col) const
        -> value_type const&;

    [[nodiscard]] auto operator()(size_type row, size_type col) -> value_type&;
    [[nodiscard]] auto operator()(size_type row, size_type col) const
        -> value_type const&;

    [[nodiscard]] auto data() const noexcept -> value_type const*;

private:
    [[nodiscard]] auto subscriptToIndex(size_type row,
                                        size_type col) const noexcept
        -> value_type
    {
        return (row * numCols_) + col;
    }

    std::unique_ptr<value_type[]> data_ {nullptr};  // NOLINT
    size_type numRows_ {};
    size_type numCols_ {};
};

template<typename T>
auto operator==(Matrix<T> const& l, Matrix<T> const& r) -> bool;

template<typename T>
auto operator!=(Matrix<T> const& l, Matrix<T> const& r) -> bool;

template<typename T>
auto operator+(Matrix<T> const& l, Matrix<T> const& r) -> Matrix<T>;

template<typename T>
auto operator+(Matrix<T> const& m, T scaler) -> Matrix<T>;

template<typename T>
auto operator+(T scaler, Matrix<T> const& m) -> Matrix<T>;

template<typename T>
auto operator-(Matrix<T> const& l, Matrix<T> const& r) -> Matrix<T>;

template<typename T>
auto operator-(Matrix<T> const& m, T scaler) -> Matrix<T>;

template<typename T>
auto operator-(T scaler, Matrix<T> const& m) -> Matrix<T>;

template<typename T>
auto operator*(Matrix<T> const& l, Matrix<T> const& r) -> Matrix<T>;

template<typename T>
auto operator*(Matrix<T> const& mat, Vector<T> const& vec) -> Vector<T>;

template<typename T>
auto operator*(Matrix<T> const& m, T scaler) -> Matrix<T>;

template<typename T>
auto operator*(T scaler, Matrix<T> const& m) -> Matrix<T>;

template<typename T>
auto operator<<(std::ostream& out, Matrix<T> const& mat) -> std::ostream&;

template<typename T>
auto compareEqual(Matrix<T> const& l, Matrix<T> const& r) -> bool;

template<typename T>
auto isSquare(Matrix<T> const& mat) -> bool;

template<typename T>
auto swapRow(Matrix<T>& mat, typename Matrix<T>::size_type a,
             typename Matrix<T>::size_type b) -> void;

template<typename T>
auto multiplyRow(Matrix<T>& mat, typename Matrix<T>::size_type row, T factor)
    -> void;

template<typename T>
auto multiplyAddRow(Matrix<T>& mat, typename Matrix<T>::size_type source,
                    typename Matrix<T>::size_type dest, T factor) -> void;

template<typename T>
auto findRowWithMaxElement(Matrix<T> const& mat,
                           typename Matrix<T>::size_type col,
                           typename Matrix<T>::size_type startRow) ->
    typename Matrix<T>::size_type;

template<typename T>
auto join(Matrix<T> const& a, Matrix<T> const& b) -> Matrix<T>;

template<typename T>
auto splitColumns(Matrix<T> const& mat, typename Matrix<T>::size_type colIdx)
    -> std::pair<Matrix<T>, Matrix<T>>;

template<typename T>
auto makeIdentity(typename Matrix<T>::size_type size) -> Matrix<T>;

template<typename T>
auto makeIdentity(Matrix<T>& mat) -> void;

template<typename T>
auto inverse(Matrix<T> const& mat) -> Matrix<T>;

template<typename T>
auto subMatrix(Matrix<T> const& mat, typename Matrix<T>::size_type rowIdx,
               typename Matrix<T>::size_type colIdx) -> Matrix<T>;

template<typename T>
auto determinant(Matrix<T> const& mat) -> typename Matrix<T>::value_type;

template<typename T>
auto isNonZero(Matrix<T> const& mat) -> bool;

template<typename T>
auto isRowEchelon(Matrix<T> const& mat) -> bool;

template<typename T>
auto rowEchelon(Matrix<T>& mat) -> void;

template<typename T>
auto rank(Matrix<T> const& mat) -> typename Matrix<T>::size_type;

/// IMPLEMENTATON
///////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix<T>::Matrix(size_type row, size_type col)
{
    resize(row, col);
}

template<typename T>
Matrix<T>::Matrix(Matrix<T> const& other)
{
    (*this) = other;
}

template<typename T>
auto Matrix<T>::operator=(Matrix<T> const& other) -> Matrix<T>&
{
    resize(other.rows(), other.cols());
    auto const* ptr = other.data_.get();
    std::copy(ptr, std::next(ptr, size()), data_.get());
    return *this;
}

template<typename T>
auto Matrix<T>::size() const noexcept -> size_type
{
    return numRows_ * numCols_;
}

template<typename T>
auto Matrix<T>::rows() const noexcept -> size_type
{
    return numRows_;
}

template<typename T>
auto Matrix<T>::cols() const noexcept -> size_type
{
    return numCols_;
}

template<typename T>
auto Matrix<T>::resize(size_type row, size_type col) -> void
{
    numRows_ = row;
    numCols_ = col;
    data_    = std::make_unique<value_type[]>(size());  // NOLINT
    clear();
}

template<typename T>
auto Matrix<T>::clear() noexcept -> void
{
    std::fill(data_.get(), std::next(data_.get(), size()), value_type {});
}

template<typename T>
auto Matrix<T>::at(size_type row, size_type col) -> value_type&
{
    if (row >= rows())
    {
        throw std::out_of_range("row index out of bounds");
    }
    if (col >= cols())
    {
        throw std::out_of_range("column index out of bounds");
    }

    return data_[subscriptToIndex(row, col)];
}

template<typename T>
auto Matrix<T>::at(size_type row, size_type col) const -> value_type const&
{
    if (row >= rows())
    {
        throw std::out_of_range("row index out of bounds");
    }
    if (col >= cols())
    {
        throw std::out_of_range("column index out of bounds");
    }

    return data_[subscriptToIndex(row, col)];
}

template<typename T>
auto Matrix<T>::operator()(size_type row, size_type col) -> value_type&
{
    return data_[subscriptToIndex(row, col)];
}

template<typename T>
auto Matrix<T>::operator()(size_type row, size_type col) const
    -> value_type const&
{
    return data_[subscriptToIndex(row, col)];
}

template<typename T>
auto Matrix<T>::data() const noexcept -> value_type const*
{
    return data_.get();
}

template<typename T>
auto operator==(Matrix<T> const& l, Matrix<T> const& r) -> bool
{
    if ((l.rows() != r.rows()) || (l.cols() != r.cols()))
    {
        return false;
    }
    return std::equal(l.data(), std::next(l.data(), l.size()), r.data());
}

template<typename T>
auto operator!=(Matrix<T> const& l, Matrix<T> const& r) -> bool
{
    return !(l == r);
}

template<typename T>
auto operator+(Matrix<T> const& l, Matrix<T> const& r) -> Matrix<T>
{
    if ((l.rows() != r.rows()) || l.cols() != r.cols())
    {
        throw std::invalid_argument("matrix layout must match");
    }

    auto tmp = Matrix<T> {l.rows(), l.cols()};
    for (decltype(tmp.rows()) row = 0; row < l.rows(); ++row)
    {
        for (decltype(tmp.cols()) col = 0; col < r.cols(); ++col)
        {
            tmp(row, col) = l(row, col) + r(row, col);
        }
    }

    return tmp;
}

template<typename T>
auto operator+(Matrix<T> const& m, T scaler) -> Matrix<T>
{
    auto tmp = Matrix<T> {m.rows(), m.cols()};
    for (decltype(tmp.rows()) row = 0; row < tmp.rows(); ++row)
    {
        for (decltype(tmp.cols()) col = 0; col < tmp.cols(); ++col)
        {
            tmp(row, col) = m(row, col) + scaler;
        }
    }
    return tmp;
}

template<typename T>
auto operator+(T scaler, Matrix<T> const& m) -> Matrix<T>
{
    return m + scaler;
}

template<typename T>
auto operator-(Matrix<T> const& l, Matrix<T> const& r) -> Matrix<T>
{
    if ((l.rows() != r.rows()) || l.cols() != r.cols())
    {
        throw std::invalid_argument("matrix layout must match");
    }

    auto tmp = Matrix<T> {l.rows(), l.cols()};
    for (decltype(tmp.rows()) row = 0; row < l.rows(); ++row)
    {
        for (decltype(tmp.cols()) col = 0; col < r.cols(); ++col)
        {
            tmp(row, col) = l(row, col) - r(row, col);
        }
    }

    return tmp;
}

template<typename T>
auto operator-(Matrix<T> const& m, T scaler) -> Matrix<T>
{
    auto tmp = Matrix<T> {m.rows(), m.cols()};
    for (decltype(tmp.rows()) row = 0; row < tmp.rows(); ++row)
    {
        for (decltype(tmp.cols()) col = 0; col < tmp.cols(); ++col)
        {
            tmp(row, col) = m(row, col) - scaler;
        }
    }
    return tmp;
}

template<typename T>
auto operator-(T scaler, Matrix<T> const& m) -> Matrix<T>
{
    return m - scaler;
}

// template<typename T>
// auto operator*(Matrix<T> const& l, Matrix<T> const& r)
//     -> Matrix<T>
// {
//     using size_type = typename Matrix<T>::size_type;
//     auto tmp        = Matrix<T> {l.rows(), l.cols()};
//     if (l.cols() == r.cols())
//     {
//         for (size_type lRow = 0; lRow < l.rows(); ++lRow)
//         {
//             for (size_type rCol = 0; rCol < r.cols(); ++rCol)
//             {
//                 auto result = T {};
//                 for (size_type lCol = 0; lCol < l.cols(); ++lCol)
//                 {
//                     auto const lIndex = (lRow * l.cols()) + lCol;
//                     auto const rIndex = (lCol * r.cols()) + rCol;
//                     result += l.data()[lIndex] + r.data()[rIndex];
//                 }

//                 auto const resultIndex  = (lRow + r.cols()) + rCol;
//                 tmp.data()[resultIndex] = result;
//             }
//         }
//     }
//     return tmp;
// }

template<typename T>
auto operator*(Matrix<T> const& m, T scaler) -> Matrix<T>
{
    auto tmp = Matrix<T> {m.rows(), m.cols()};
    for (decltype(tmp.rows()) row = 0; row < tmp.rows(); ++row)
    {
        for (decltype(tmp.cols()) col = 0; col < tmp.cols(); ++col)
        {
            tmp(row, col) = m(row, col) * scaler;
        }
    }
    return tmp;
}

template<typename T>
auto operator*(T scaler, Matrix<T> const& m) -> Matrix<T>
{
    return m * scaler;
}

template<typename T>
auto operator*(Matrix<T> const& mat, Vector<T> const& vec) -> Vector<T>
{
    if (mat.cols() != vec.size())
    {
        throw std::domain_error("matrix columns and vector size must match");
    }

    auto result = Vector<T> {vec};
    for (decltype(mat.rows()) row = 0; row < mat.rows(); ++row)
    {
        auto sum = T {};
        for (decltype(mat.cols()) col = 0; col < mat.cols(); ++col)
        {
            sum += mat(row, col) * vec[col];
        }
        result[row] = sum;
    }

    return result;
}

template<typename T>
auto operator<<(std::ostream& out, Matrix<T> const& m) -> std::ostream&
{
    using size_type = typename Matrix<T>::size_type;
    for (size_type row = 0; row < m.rows(); ++row)
    {
        for (size_type col = 0; col < m.cols(); ++col)
        {
            out << m(row, col) << ' ';
        }
        out << '\n';
    }
    return out;
}

template<typename T>
auto isSquare(Matrix<T> const& mat) -> bool
{
    return mat.rows() == mat.cols();
}

template<typename T>
auto swapRow(Matrix<T>& mat, typename Matrix<T>::size_type a,
             typename Matrix<T>::size_type b) -> void
{
    if ((a >= mat.rows()) || (b >= mat.rows()))
    {
        throw std::out_of_range("row index out of bounds");
    }

    for (decltype(mat.cols()) col = 0; col < mat.cols(); ++col)
    {
        std::swap(mat(a, col), mat(b, col));
    }
}

template<typename T>
auto multiplyRow(Matrix<T>& mat, typename Matrix<T>::size_type row, T factor)
    -> void
{
    if (row >= mat.rows())
    {
        throw std::out_of_range("row index out of bounds");
    }

    for (decltype(mat.cols()) col = 0; col < mat.cols(); ++col)
    {
        mat(row, col) *= factor;
    }
}

template<typename T>
auto multiplyAddRow(Matrix<T>& mat, typename Matrix<T>::size_type source,
                    typename Matrix<T>::size_type dest, T factor) -> void
{
    if ((source >= mat.rows()) || (source >= mat.rows()))
    {
        throw std::out_of_range("row index out of bounds");
    }

    for (decltype(mat.cols()) col = 0; col < mat.cols(); ++col)
    {
        mat(dest, col) += mat(source, col) * factor;
    }
}

template<typename T>
auto findRowWithMaxElement(Matrix<T> const& mat,
                           typename Matrix<T>::size_type col,
                           typename Matrix<T>::size_type startRow) ->
    typename Matrix<T>::size_type
{
    using size_type = typename Matrix<T>::size_type;

    if (startRow >= mat.rows())
    {
        throw std::out_of_range("row index out of bounds");
    }
    if (col >= mat.cols())
    {
        throw std::out_of_range("column index out of bounds");
    }

    size_type maxId = startRow;
    for (size_type row = startRow; row < mat.rows(); ++row)
    {
        if (mat(row, col) >= mat(maxId, col))
        {
            maxId = row;
        }
    }

    return maxId;
}

template<typename T>
auto join(Matrix<T> const& a, Matrix<T> const& b) -> Matrix<T>
{
    using size_type = typename Matrix<T>::size_type;

    if (a.rows() != b.rows())
    {
        throw std::invalid_argument("rows size must match");
    }

    auto result = Matrix<T> {a.rows(), a.cols() + b.cols()};

    for (size_type row = 0; row < result.rows(); ++row)
    {
        for (size_type col = 0; col < result.cols(); ++col)
        {
            auto const& source   = col < a.cols() ? a : b;
            auto const sourceCol = col < a.cols() ? col : col - a.cols();
            result(row, col)     = source(row, sourceCol);
        }
    }

    return result;
}

template<typename T>
auto splitColumns(Matrix<T> const& mat, typename Matrix<T>::size_type colIdx)
    -> std::pair<Matrix<T>, Matrix<T>>
{
    using size_type = typename Matrix<T>::size_type;

    if (colIdx >= mat.cols())
    {
        throw std::out_of_range("column index out of bounds");
    }

    auto const numRows = mat.rows();
    auto const numColA = colIdx;
    auto const numColB = mat.cols() - colIdx;

    auto a = Matrix<T> {numRows, numColA};
    auto b = Matrix<T> {numRows, numColB};

    for (size_type row = 0; row < numRows; ++row)
    {
        for (size_type col = 0; col < mat.cols(); ++col)
        {
            if (col < colIdx)
            {
                a(row, col) = mat(row, col);
            }
            else
            {
                b(row, col - colIdx) = mat(row, col);
            }
        }
    }

    return std::make_pair(a, b);
}

template<typename T>
auto makeIdentity(typename Matrix<T>::size_type size) -> Matrix<T>
{
    auto mat = Matrix<T> {size, size};
    makeIdentity(mat);
    return mat;
}

template<typename T>
auto makeIdentity(Matrix<T>& mat) -> void
{
    if (!isSquare(mat))
    {
        throw std::invalid_argument("matrix must be square");
    }

    mat.clear();
    for (decltype(mat.rows()) i = 0; i < mat.rows(); ++i)
    {
        mat(i, i) = T {1};
    }
}

template<typename T>
auto inverse(Matrix<T> const& mat) -> Matrix<T>
{
    using size_type = typename Matrix<T>::size_type;

    if (!isSquare(mat))
    {
        throw std::invalid_argument("matrix must be square");
    }

    static constexpr auto maxLoopCount = size_type {100};

    auto const identity = makeIdentity<T>(mat.rows());
    auto augment        = join(mat, identity);

    auto count     = size_type {0};
    auto completed = false;

    auto cRow = size_type {};
    auto cCol = size_type {};

    auto result = Matrix<T> {mat.rows(), mat.cols()};
    while ((!completed) && (count < maxLoopCount))
    {
        for (auto diagIdx = size_type {0}; diagIdx < augment.rows(); ++diagIdx)
        {
            cRow = diagIdx;
            cCol = diagIdx;

            if (auto maxRowIdx = findRowWithMaxElement(augment, cCol, cRow);
                maxRowIdx != cRow)
            {
                swapRow(augment, cRow, maxRowIdx);
            }

            if (augment(cRow, cCol) != T {1})
            {
                auto factor = T {1} / augment(cRow, cCol);
                multiplyRow(augment, cRow, factor);
            }

            for (auto rowIdx = cRow + 1; rowIdx < augment.rows(); ++rowIdx)
            {
                auto rowOneIndex  = cCol;
                auto currentValue = augment(rowIdx, cCol);
                auto rowOneValue  = augment(rowOneIndex, cCol);

                if (!closeEnough(rowOneValue, T {}))
                {
                    auto correction = -(currentValue / rowOneValue);
                    multiplyAddRow(augment, rowOneIndex, rowIdx, correction);
                }
            }

            for (auto colIdx = cCol + 1; colIdx < mat.cols(); ++colIdx)
            {
                if (!closeEnough(augment(cRow, colIdx), T {}))
                {
                    auto rowOneIndex  = colIdx;
                    auto currentValue = augment(cRow, colIdx);
                    auto rowOneValue  = augment(rowOneIndex, colIdx);

                    if (!closeEnough(rowOneValue, T {}))
                    {
                        auto correction = -(currentValue / rowOneValue);
                        multiplyAddRow(augment, rowOneIndex, cRow, correction);
                    }
                }
            }
        }

        if (auto splits = splitColumns(augment, mat.cols());
            compareEqual(splits.first, identity))
        {
            completed = true;
            result    = splits.second;
        }

        count += 1;
    }

    return result;
}

template<typename T>
auto compareEqual(Matrix<T> const& l, Matrix<T> const& r) -> bool
{
    if ((l.rows() != r.rows()) || (l.cols() != r.cols()))
    {
        return false;
    }

    auto cmp = [](auto a, auto b) { return closeEnough(a, b); };
    return std::equal(l.data(), std::next(l.data(), l.size()), r.data(), cmp);
}

template<typename T>
auto subMatrix(Matrix<T> const& mat, typename Matrix<T>::size_type rowIdx,
               typename Matrix<T>::size_type colIdx) -> Matrix<T>
{
    auto result = Matrix<T> {mat.rows() - 1, mat.cols() - 1};
    for (decltype(mat.rows()) row = 0; row < mat.rows(); ++row)
    {
        for (decltype(mat.rows()) col = 0; col < mat.cols(); ++col)
        {
            if ((row != rowIdx) && (col != colIdx))
            {
                auto newRow            = row < rowIdx ? row : row - 1;
                auto newCol            = col < colIdx ? col : col - 1;
                result(newRow, newCol) = mat(row, col);
            }
        }
    }
    return result;
}

template<typename T>
auto determinant(Matrix<T> const& mat) -> typename Matrix<T>::value_type
{
    using value_type = typename Matrix<T>::value_type;

    if (!isSquare(mat))
    {
        throw std::invalid_argument("matrix must be square");
    }

    if (mat.rows() == 2)
    {
        return (mat(0, 0) * mat(1, 1)) - (mat(0, 1) * mat(1, 0));
    }

    auto sum  = value_type {0};
    auto sign = value_type {1};
    for (decltype(mat.cols()) col = 0; col < mat.cols(); ++col)
    {
        auto sub = subMatrix(mat, 0, col);
        auto d   = determinant(sub);
        sum += mat(0, col) * d * sign;
        sign = -sign;
    }

    return sum;
}

template<typename T>
auto isNonZero(Matrix<T> const& mat) -> bool
{
    for (decltype(mat.rows()) row {0}; row < mat.rows(); ++row)
    {
        for (decltype(mat.cols()) col {0}; col < mat.cols(); ++col)
        {
            if (!closeEnough(mat(row, col), T {}))
            {
                return true;
            }
        }
    }
    return false;
}

template<typename T>
auto isRowEchelon(Matrix<T> const& mat) -> bool
{
    auto sum = T {};
    for (decltype(mat.rows()) row {1}; row < mat.rows(); ++row)
    {
        for (decltype(mat.cols()) col {0}; col < row; ++col)
        {
            sum += mat(row, col);
        }
    }

    return closeEnough(sum, T {});
}

template<typename T>
auto rowEchelon(Matrix<T>& mat) -> void
{
    if (mat.cols() < mat.rows())
    {
        throw std::invalid_argument(
            "matrix must have at least have as many columns as rows");
    }

    using size_type                    = typename Matrix<T>::size_type;
    static constexpr auto maxLoopCount = size_type {100};

    auto count     = size_type {0};
    auto completed = false;

    auto cRow = size_type {};
    auto cCol = size_type {};

    while ((!completed) && (count < maxLoopCount))
    {
        for (auto diagIdx = size_type {0}; diagIdx < mat.rows(); ++diagIdx)
        {
            cRow = diagIdx;
            cCol = diagIdx;

            for (auto rowIdx = cRow + 1; rowIdx < mat.rows(); ++rowIdx)
            {
                if (!closeEnough(mat(rowIdx, cCol), T {}))
                {
                    auto const rowOneIndex  = cCol;
                    auto const rowOneValue  = mat(rowOneIndex, cCol);
                    auto const currentValue = mat(rowIdx, cCol);
                    if (!closeEnough(rowOneValue, T {}))
                    {
                        auto const correction = -(currentValue / rowOneValue);
                        multiplyAddRow(mat, rowOneIndex, rowIdx, correction);
                    }
                }
            }
        }

        completed = isRowEchelon(mat);
        count += 1;
    }
}

template<typename T>
auto rank(Matrix<T> const& mat) -> typename Matrix<T>::size_type
{
    using size_type = typename Matrix<T>::size_type;

    auto copy = Matrix<T> {mat};
    rowEchelon(copy);

    auto numNonZeroRows = size_type {};
    if (!isRowEchelon(copy))
    {
        auto subMatricies = std::vector<Matrix<T>> {};
        subMatricies.push_back(mat);

        auto completed      = false;
        auto subMatrixCount = size_type {};
        while ((subMatrixCount < subMatricies.size()) && (!completed))
        {
            auto const cur = subMatricies[subMatrixCount];
            subMatrixCount++;

            if (isNonZero(cur))
            {
                if (!closeEnough(determinant(cur), T {}))
                {
                    completed      = true;
                    numNonZeroRows = cur.rows();
                }
                else
                {
                    if ((cur.rows() > 2) && (cur.cols() > 2))
                    {
                        for (auto i = size_type {0}; i < cur.rows(); ++i)
                        {
                            for (auto j = size_type {0}; j < cur.cols(); ++j)
                            {
                                auto sub = subMatrix(cur, i, j);
                                subMatricies.push_back(sub);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (decltype(copy.rows()) row = size_type {0}; row < copy.rows();
             ++row)
        {
            auto colSum = size_type {};
            for (decltype(copy.cols()) col = 0; col < copy.cols(); ++col)
            {
                if (!closeEnough(copy(row, col), T {}))
                {
                    colSum++;
                }
            }

            if (colSum > 0)
            {
                numNonZeroRows++;
            }
        }
    }

    return numNonZeroRows;
}
}  // namespace math