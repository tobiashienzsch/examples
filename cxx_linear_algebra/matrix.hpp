#pragma once

#include "vector.hpp"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>

namespace ta
{

template<typename T>
struct DynamicMatrix
{
    using value_type = T;
    using size_type  = std::uint32_t;

    DynamicMatrix() noexcept = default;
    DynamicMatrix(size_type rows, size_type cols);

    auto clear() noexcept -> void;
    auto resize(size_type rows, size_type cols) -> void;

    [[nodiscard]] auto size() const noexcept -> size_type;
    [[nodiscard]] auto rows() const noexcept -> size_type;
    [[nodiscard]] auto cols() const noexcept -> size_type;

    [[nodiscard]] auto at(size_type row, size_type col) -> value_type&;
    [[nodiscard]] auto at(size_type row, size_type col) const
        -> value_type const&;

    [[nodiscard]] auto data() noexcept -> value_type*;
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
auto operator==(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r) -> bool;

template<typename T>
auto operator!=(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r) -> bool;

template<typename T>
auto operator+(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r)
    -> DynamicMatrix<T>;

template<typename T>
auto operator+(DynamicMatrix<T> const& m, T scaler) -> DynamicMatrix<T>;

template<typename T>
auto operator+(T scaler, DynamicMatrix<T> const& m) -> DynamicMatrix<T>;

template<typename T>
auto operator-(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r)
    -> DynamicMatrix<T>;

template<typename T>
auto operator-(DynamicMatrix<T> const& m, T scaler) -> DynamicMatrix<T>;

template<typename T>
auto operator-(T scaler, DynamicMatrix<T> const& m) -> DynamicMatrix<T>;

template<typename T>
auto operator*(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r)
    -> DynamicMatrix<T>;

template<typename T>
auto operator*(DynamicMatrix<T> const& mat, DynamicVector<T> const& vec)
    -> DynamicVector<T>;

template<typename T>
auto operator*(DynamicMatrix<T> const& m, T scaler) -> DynamicMatrix<T>;

template<typename T>
auto operator*(T scaler, DynamicMatrix<T> const& m) -> DynamicMatrix<T>;

template<typename T>
auto operator<<(std::ostream& out, DynamicMatrix<T> const& mat)
    -> std::ostream&;

/// IMPL
template<typename T>
DynamicMatrix<T>::DynamicMatrix(size_type row, size_type col)
{
    resize(row, col);
}

template<typename T>
auto DynamicMatrix<T>::size() const noexcept -> size_type
{
    return numRows_ * numCols_;
}

template<typename T>
auto DynamicMatrix<T>::rows() const noexcept -> size_type
{
    return numRows_;
}

template<typename T>
auto DynamicMatrix<T>::cols() const noexcept -> size_type
{
    return numCols_;
}

template<typename T>
auto DynamicMatrix<T>::resize(size_type row, size_type col) -> void
{
    numRows_ = row;
    numCols_ = col;
    data_    = std::make_unique<value_type[]>(size());  // NOLINT
    clear();
}

template<typename T>
auto DynamicMatrix<T>::clear() noexcept -> void
{
    std::fill(data_.get(), std::next(data_.get(), size()), value_type {});
}

template<typename T>
auto DynamicMatrix<T>::at(size_type row, size_type col) -> value_type&
{
    return data_[subscriptToIndex(row, col)];
}

template<typename T>
auto DynamicMatrix<T>::at(size_type row, size_type col) const
    -> value_type const&
{
    return data_[subscriptToIndex(row, col)];
}

template<typename T>
auto DynamicMatrix<T>::data() noexcept -> value_type*
{
    return data_.get();
}

template<typename T>
auto DynamicMatrix<T>::data() const noexcept -> value_type const*
{
    return data_.get();
}

template<typename T>
auto operator==(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r) -> bool
{
    if ((l.rows() != r.rows()) || (l.cols() != r.cols()))
    {
        return false;
    }
    return std::equal(l.data(), std::next(l.data(), l.size()), r.data());
}

template<typename T>
auto operator!=(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r) -> bool
{
    return !(l == r);
}

template<typename T>
auto operator+(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r)
    -> DynamicMatrix<T>
{
    auto tmp = DynamicMatrix<T> {l.rows(), l.cols()};
    std::transform(l.data(), std::next(l.data(), l.size()), r.data(),
                   tmp.data(), std::plus<> {});
    return tmp;
}

template<typename T>
auto operator+(DynamicMatrix<T> const& m, T scaler) -> DynamicMatrix<T>
{
    auto tmp  = DynamicMatrix<T> {m.rows(), m.cols()};
    auto plus = [scaler](auto v) { return v + scaler; };
    std::transform(m.data(), std::next(m.data(), m.size()), tmp.data(), plus);
    return tmp;
}

template<typename T>
auto operator+(T scaler, DynamicMatrix<T> const& m) -> DynamicMatrix<T>
{
    return m + scaler;
}

template<typename T>
auto operator-(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r)
    -> DynamicMatrix<T>
{
    auto tmp = DynamicMatrix<T> {l.rows(), l.cols()};
    std::transform(l.data(), std::next(l.data(), l.size()), r.data(),
                   tmp.data(), std::minus<> {});
    return tmp;
}

template<typename T>
auto operator-(DynamicMatrix<T> const& m, T scaler) -> DynamicMatrix<T>
{
    auto tmp   = DynamicMatrix<T> {m.rows(), m.cols()};
    auto minus = [scaler](auto v) { return v - scaler; };
    std::transform(m.data(), std::next(m.data(), m.size()), tmp.data(), minus);
    return tmp;
}

template<typename T>
auto operator-(T scaler, DynamicMatrix<T> const& m) -> DynamicMatrix<T>
{
    return m - scaler;
}

template<typename T>
auto operator*(DynamicMatrix<T> const& l, DynamicMatrix<T> const& r)
    -> DynamicMatrix<T>
{
    using size_type = typename DynamicMatrix<T>::size_type;
    auto tmp        = DynamicMatrix<T> {l.rows(), l.cols()};
    if (l.cols() == r.cols())
    {
        for (size_type lRow = 0; lRow < l.rows(); ++lRow)
        {
            for (size_type rCol = 0; rCol < r.cols(); ++rCol)
            {
                auto result = T {};
                for (size_type lCol = 0; lCol < l.cols(); ++lCol)
                {
                    auto const lIndex = (lRow * l.cols()) + lCol;
                    auto const rIndex = (lCol * r.cols()) + rCol;
                    result += l.data()[lIndex] + r.data()[rIndex];
                }

                auto const resultIndex  = (lRow + r.cols()) + rCol;
                tmp.data()[resultIndex] = result;
            }
        }
    }
    return tmp;
}

template<typename T>
auto operator*(DynamicMatrix<T> const& m, T scaler) -> DynamicMatrix<T>
{
    auto tmp      = DynamicMatrix<T> {m.rows(), m.cols()};
    auto multiply = [scaler](auto v) { return v * scaler; };
    std::transform(m.data(), std::next(m.data(), m.size()), tmp.data(),
                   multiply);
    return tmp;
}

template<typename T>
auto operator*(T scaler, DynamicMatrix<T> const& m) -> DynamicMatrix<T>
{
    return m * scaler;
}

template<typename T>
auto operator*(DynamicMatrix<T> const& mat, DynamicVector<T> const& vec)
    -> DynamicVector<T>
{
    if (mat.cols() != vec.size())
    {
        throw std::domain_error("matrix columns and vector size must match");
    }

    auto result = DynamicVector<T> {vec};
    for (decltype(mat.rows()) row = 0; row < mat.rows(); ++row)
    {
        auto sum = T {};
        for (decltype(mat.cols()) col = 0; col < mat.cols(); ++col)
        {
            sum += mat.at(row, col) * vec[col];
        }
        result[row] = sum;
    }

    return result;
}

template<typename T>
auto operator<<(std::ostream& out, DynamicMatrix<T> const& m) -> std::ostream&
{
    using size_type = typename DynamicMatrix<T>::size_type;
    for (size_type row = 0; row < m.rows(); ++row)
    {
        for (size_type col = 0; col < m.cols(); ++col)
        {
            out << m.at(row, col) << ' ';
        }
        out << '\n';
    }
    return out;
}

}  // namespace ta