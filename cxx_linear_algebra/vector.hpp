#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <ostream>
#include <stdexcept>

namespace math
{

namespace detail
{
template<typename T>
[[nodiscard]] auto closeEnough(T a, T b) -> bool
{
    return std::abs(a - b) < (std::numeric_limits<T>::epsilon() * T {16});
}
}  // namespace detail

template<typename T>
struct Vector
{
    using value_type = T;
    using size_type  = std::uint32_t;

    Vector() noexcept = default;
    explicit Vector(size_type size);
    Vector(Vector<T> const& other);

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
auto operator==(Vector<T> const& l, Vector<T> const& r) -> bool;

template<typename T>
auto operator!=(Vector<T> const& l, Vector<T> const& r) -> bool;

template<typename T>
auto operator+(Vector<T> const& l, Vector<T> const& r) -> Vector<T>;

template<typename T>
auto operator-(Vector<T> const& l, Vector<T> const& r) -> Vector<T>;

template<typename T>
auto operator*(Vector<T> const& vec, T const& scaler) -> Vector<T>;

template<typename T>
auto operator*(T const& scaler, Vector<T> const& vec) -> Vector<T>;

template<typename T>
auto operator<<(std::ostream& out, Vector<T> const& vec) -> std::ostream&;

template<typename T>
auto dotProduct(Vector<T> const& l, Vector<T> const& r) -> T;

template<typename T>
auto crossProduct(Vector<T> const& l, Vector<T> const& r) -> Vector<T>;

template<typename T>
auto norm(Vector<T> const& vec) noexcept -> typename Vector<T>::value_type;

template<typename T>
auto normalized(Vector<T> const& vec) noexcept -> Vector<T>;

template<typename T>
auto normalize(Vector<T>& vec) noexcept -> void;

// IMPLEMENTATION
template<typename T>
Vector<T>::Vector(size_type size)
{
    resize(size);
}

template<typename T>
Vector<T>::Vector(Vector<T> const& other)
{
    resize(other.size());
    auto const* ptr = other.data_.get();
    std::copy(ptr, std::next(ptr, size()), data_.get());
}

template<typename T>
auto Vector<T>::clear() noexcept -> void
{
    std::fill(data_.get(), std::next(data_.get(), size()), value_type {});
}

template<typename T>
auto Vector<T>::resize(size_type size) -> void
{
    size_ = size;
    data_ = std::make_unique<value_type[]>(this->size());  // NOLINT
    clear();
}

template<typename T>
auto Vector<T>::size() const noexcept -> size_type
{
    return size_;
}

template<typename T>
auto Vector<T>::at(size_type idx) -> value_type&
{
    if (idx >= size())
    {
        throw std::out_of_range("index out of bounds");
    }
    return data_[idx];
}

template<typename T>
auto Vector<T>::at(size_type idx) const -> value_type const&
{
    if (idx >= size())
    {
        throw std::out_of_range("index out of bounds");
    }
    return data_[idx];
}

template<typename T>
auto Vector<T>::operator[](size_type idx) -> value_type&
{
    return data_[idx];
}

template<typename T>
auto Vector<T>::operator[](size_type idx) const -> value_type const&
{
    return data_[idx];
}

template<typename T>
auto operator==(Vector<T> const& l, Vector<T> const& r) -> bool
{
    if (l.size() != r.size())
    {
        return false;
    }
    using size_type = typename Vector<T>::size_type;
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
auto operator!=(Vector<T> const& l, Vector<T> const& r) -> bool
{
    return !(l == r);
}

template<typename T>
auto operator+(Vector<T> const& l, Vector<T> const& r) -> Vector<T>
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }

    auto result = Vector<T> {l.size()};
    for (decltype(l.size()) i = 0; i < l.size(); ++i)
    {
        result[i] = l[i] + r[i];
    }
    return result;
}

template<typename T>
auto operator-(Vector<T> const& l, Vector<T> const& r) -> Vector<T>
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }

    auto result = Vector<T> {l.size()};
    for (decltype(l.size()) i = 0; i < l.size(); ++i)
    {
        result[i] = l[i] - r[i];
    }
    return result;
}

template<typename T>
auto operator*(Vector<T> const& vec, T const& scaler) -> Vector<T>
{
    auto result = Vector<T> {vec.size()};
    for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
    {
        result[i] = vec[i] * scaler;
    }
    return result;
}

template<typename T>
auto operator*(T const& scaler, Vector<T> const& vec) -> Vector<T>
{
    return vec * scaler;
}

template<typename T>
auto operator<<(std::ostream& out, Vector<T> const& vec) -> std::ostream&
{
    using size_type = typename Vector<T>::size_type;
    for (size_type i = 0; i < vec.size(); ++i)
    {
        out << vec[i] << ' ';
    }
    return out;
}

template<typename T>
auto dotProduct(Vector<T> const& l, Vector<T> const& r) -> T
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }

    auto result     = T {};
    using size_type = typename Vector<T>::size_type;
    for (size_type i = 0; i < l.size(); ++i)
    {
        result += l[i] * r[i];
    }

    return result;
}

template<typename T>
auto crossProduct(Vector<T> const& l, Vector<T> const& r) -> Vector<T>
{
    if (l.size() != r.size())
    {
        throw std::domain_error("vectors need to be the same size");
    }
    if (l.size() != 3)
    {
        throw std::domain_error("only 3-dimensional vector supported");
    }

    auto result = Vector<T> {l.size()};
    result[0]   = (l[1] * r[2]) - (l[2] * r[1]);
    result[1]   = (l[0] * r[2]) - (l[2] * r[0]);
    result[2]   = (l[0] * r[1]) - (l[1] * r[0]);
    return result;
}

template<typename T>
auto norm(Vector<T> const& vec) noexcept -> typename Vector<T>::value_type
{
    using value_type = typename Vector<T>::value_type;
    auto sum         = value_type {};
    for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
    }
    return std::sqrt(sum);
}

template<typename T>
auto normalized(Vector<T> const& vec) noexcept -> Vector<T>
{
    using value_type = typename Vector<T>::value_type;
    return Vector<T> {vec} * (value_type {1} / norm(vec));
}

template<typename T>
auto normalize(Vector<T>& vec) noexcept -> void
{
    using value_type = typename Vector<T>::value_type;
    auto const n     = norm(vec);
    for (decltype(vec.size()) i = 0; i < vec.size(); ++i)
    {
        vec[i] = vec[i] * (value_type {1} / n);
    }
}

}  // namespace math