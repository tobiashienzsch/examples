#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <iterator>
#include <memory>
#include <numeric>
#include <ostream>
#include <stdexcept>

namespace ta
{
template<typename T>
struct DynamicVector
{
    using value_type = T;
    using size_type  = std::uint32_t;

    DynamicVector() noexcept = default;
    explicit DynamicVector(size_type size);
    DynamicVector(DynamicVector<T> const& other);

    auto clear() noexcept -> void;
    auto resize(size_type size) -> void;

    [[nodiscard]] auto size() const noexcept -> size_type;

    [[nodiscard]] auto at(size_type idx) -> value_type&;
    [[nodiscard]] auto at(size_type idx) const -> value_type const&;

    [[nodiscard]] auto operator[](size_type idx) -> value_type&;
    [[nodiscard]] auto operator[](size_type idx) const -> value_type const&;

private:
    std::unique_ptr<value_type[]> data_ {};  // NOLINT
    size_type size_ {};
};

template<typename T>
auto operator==(DynamicVector<T> const& l, DynamicVector<T> const& r) -> bool;

template<typename T>
auto operator!=(DynamicVector<T> const& l, DynamicVector<T> const& r) -> bool;

template<typename T>
auto operator+(DynamicVector<T> const& l, DynamicVector<T> const& r)
    -> DynamicVector<T>;

template<typename T>
auto operator-(DynamicVector<T> const& l, DynamicVector<T> const& r)
    -> DynamicVector<T>;

template<typename T>
auto operator*(DynamicVector<T> const& vec, T const& scaler)
    -> DynamicVector<T>;

template<typename T>
auto operator*(T const& scaler, DynamicVector<T> const& vec)
    -> DynamicVector<T>;

template<typename T>
auto operator<<(std::ostream& out, DynamicVector<T> const& vec)
    -> std::ostream&;

template<typename T>
auto dotProduct(DynamicVector<T> const& l, DynamicVector<T> const& r) -> T;

template<typename T>
auto crossProduct(DynamicVector<T> const& l, DynamicVector<T> const& r)
    -> DynamicVector<T>;

template<typename T>
auto norm(DynamicVector<T> const& vec) noexcept ->
    typename DynamicVector<T>::value_type;

template<typename T>
auto normalized(DynamicVector<T> const& vec) noexcept -> DynamicVector<T>;

template<typename T>
auto normalize(DynamicVector<T>& vec) noexcept -> void;

// IMPLEMENTATION
template<typename T>
DynamicVector<T>::DynamicVector(size_type size)
{
    resize(size);
}

template<typename T>
DynamicVector<T>::DynamicVector(DynamicVector<T> const& other)
{
    resize(other.size());
    auto const* ptr = other.data_.get();
    std::copy(ptr, std::next(ptr, size()), data_.get());
}

template<typename T>
auto DynamicVector<T>::clear() noexcept -> void
{
    std::fill(data_.get(), std::next(data_.get(), size()), value_type {});
}

template<typename T>
auto DynamicVector<T>::resize(size_type size) -> void
{
    size_ = size;
    data_ = std::make_unique<value_type[]>(this->size());  // NOLINT
    clear();
}

template<typename T>
auto DynamicVector<T>::size() const noexcept -> size_type
{
    return size_;
}

template<typename T>
auto DynamicVector<T>::at(size_type idx) -> value_type&
{
    if (idx >= size())
    {
        throw std::out_of_range("index out of bounds");
    }
    return data_[idx];
}

template<typename T>
auto DynamicVector<T>::at(size_type idx) const -> value_type const&
{
    if (idx >= size())
    {
        throw std::out_of_range("index out of bounds");
    }
    return data_[idx];
}

template<typename T>
auto DynamicVector<T>::operator[](size_type idx) -> value_type&
{
    return data_[idx];
}

template<typename T>
auto DynamicVector<T>::operator[](size_type idx) const -> value_type const&
{
    return data_[idx];
}

template<typename T>
auto operator==(DynamicVector<T> const& l, DynamicVector<T> const& r) -> bool
{
    if (l.size() != r.size())
    {
        return false;
    }
    using size_type = typename DynamicVector<T>::size_type;
    for (size_type i = 0; i < l.size(); ++i)
    {
        if (l[i] != r[i])
        {
            return false;
        }
    }
    return true;
}

template<typename T>
auto operator!=(DynamicVector<T> const& l, DynamicVector<T> const& r) -> bool
{
    return !(l == r);
}

template<typename T>
auto operator+(DynamicVector<T> const& l, DynamicVector<T> const& r)
    -> DynamicVector<T>
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }

    auto result = DynamicVector<T> {l.size()};
    for (decltype(l.size()) i = 0; i < l.size(); ++i)
    {
        result[i] = l[i] + r[i];
    }
    return result;
}

template<typename T>
auto operator-(DynamicVector<T> const& l, DynamicVector<T> const& r)
    -> DynamicVector<T>
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }

    auto result = DynamicVector<T> {l.size()};
    for (decltype(l.size()) i = 0; i < l.size(); ++i)
    {
        result[i] = l[i] - r[i];
    }
    return result;
}

template<typename T>
auto operator*(DynamicVector<T> const& vec, T const& scaler) -> DynamicVector<T>
{
    auto result = DynamicVector<T> {vec.size()};
    for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
    {
        result[i] = vec[i] * scaler;
    }
    return result;
}

template<typename T>
auto operator*(T const& scaler, DynamicVector<T> const& vec) -> DynamicVector<T>
{
    return vec * scaler;
}

template<typename T>
auto operator<<(std::ostream& out, DynamicVector<T> const& vec) -> std::ostream&
{
    using size_type = typename DynamicVector<T>::size_type;
    for (size_type i = 0; i < vec.size(); ++i)
    {
        out << vec[i] << ' ';
    }
    return out;
}

template<typename T>
auto dotProduct(DynamicVector<T> const& l, DynamicVector<T> const& r) -> T
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }

    auto result     = T {};
    using size_type = typename DynamicVector<T>::size_type;
    for (size_type i = 0; i < l.size(); ++i)
    {
        result += l[i] * r[i];
    }

    return result;
}

template<typename T>
auto crossProduct(DynamicVector<T> const& l, DynamicVector<T> const& r)
    -> DynamicVector<T>
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }
    if (l.size() != 3)
    {
        throw std::domain_error("only 3-dimensional vector supported");
    }

    auto result = DynamicVector<T> {l.size()};
    result[0]   = (l[1] * r[2]) - (l[2] * r[1]);
    result[1]   = (l[0] * r[2]) - (l[2] * r[0]);
    result[2]   = (l[0] * r[1]) - (l[1] * r[0]);
    return result;
}

template<typename T>
auto norm(DynamicVector<T> const& vec) noexcept ->
    typename DynamicVector<T>::value_type
{
    using value_type = typename DynamicVector<T>::value_type;
    auto sum         = value_type {};
    for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
    }
    return std::sqrt(sum);
}

template<typename T>
auto normalized(DynamicVector<T> const& vec) noexcept -> DynamicVector<T>
{
    using value_type = typename DynamicVector<T>::value_type;
    return DynamicVector<T> {vec} * (value_type {1} / norm(vec));
}

template<typename T>
auto normalize(DynamicVector<T>& vec) noexcept -> void
{
    using value_type = typename DynamicVector<T>::value_type;
    auto const n     = norm(vec);
    for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
    {
        vec[i] = vec[i] * (value_type {1} / n);
    }
}

}  // namespace ta