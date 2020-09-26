#pragma once

#include <array>        // for trivial storage
#include <cassert>      // for assertion diagnostics
#include <cstddef>      // for size_t
#include <cstdint>      // for fixed-width integer types
#include <functional>   // for less and equal_to
#include <iterator>     // for reverse_iterator and iterator traits
#include <limits>       // for numeric_limits
#include <type_traits>  // for aligned_storage and all meta-functions

namespace etl
{
namespace detail
{

template<typename T>
inline constexpr bool is_movable_v
    = std::is_object_v<T>&& std::is_assignable_v<T&, T>&& std::is_move_constructible_v<T>&& std::is_swappable_v<T&>;

template<typename Rng>
using range_iterator_t = decltype(std::begin(std::declval<Rng>()));

template<typename T>
using iterator_reference_t = typename std::iterator_traits<T>::reference;

template<typename T>
using iterator_category_t = typename std::iterator_traits<T>::iterator_category;

template<typename T, typename Cat, typename = void>
struct Iterator_ : std::false_type
{
};

template<typename T, typename Cat>
struct Iterator_<T, Cat, std::void_t<iterator_category_t<T>>>
    : std::bool_constant<std::is_convertible_v<iterator_category_t<T>, Cat>>
{
};

// Concepts (poor-man emulation using type traits)
template<typename T>
static constexpr bool InputIterator = Iterator_<T, std::input_iterator_tag> {};

template<typename T>
static constexpr bool ForwardIterator = Iterator_<T, std::forward_iterator_tag> {};

template<typename T>
static constexpr bool OutputIterator = Iterator_<T, std::output_iterator_tag> {} || ForwardIterator<T>;

template<typename T>
static constexpr bool BidirectionalIterator = Iterator_<T, std::bidirectional_iterator_tag> {};

template<typename T>
static constexpr bool RandomAccessIterator = Iterator_<T, std::random_access_iterator_tag> {};

template<typename T>
static constexpr bool RandomAccessRange = RandomAccessIterator<range_iterator_t<T>>;

/**
 * @brief Smallest fixed-width unsigned integer type that can represent values in the range [0, N].
 */
// clang-format off
template<size_t N>
using smallest_size_t =
            std::conditional_t<(N < std::numeric_limits<uint8_t>::max()),  uint8_t,
            std::conditional_t<(N < std::numeric_limits<uint16_t>::max()), uint16_t,
            std::conditional_t<(N < std::numeric_limits<uint32_t>::max()), uint32_t,
            std::conditional_t<(N < std::numeric_limits<uint64_t>::max()), uint64_t,
                                                                 size_t>>>>;
// clang-format on

/**
 * @brief Index a range doing bound checks in debug builds FCV_REQUIRES_(RandomAccessRange<Rng>)
 */
template<typename Rng, typename Index>
constexpr decltype(auto) index(Rng&& rng, Index&& i) noexcept
{
    assert(static_cast<ptrdiff_t>(i) < (std::end(rng) - std::begin(rng)));
    return std::begin(std::forward<Rng>(rng))[std::forward<Index>(i)];
}

/**
 * @brief Storage for zero elements.
 */
template<typename T>
class static_vector_zero_storage
{
public:
    using size_type       = uint8_t;
    using value_type      = T;
    using difference_type = ptrdiff_t;
    using pointer         = T*;
    using const_pointer   = T const*;

    /**
     * @brief Defaulted constructor.
     */
    constexpr static_vector_zero_storage() = default;

    /**
     * @brief Defaulted copy constructor.
     */
    constexpr static_vector_zero_storage(static_vector_zero_storage const&) = default;

    /**
     * @brief Defaulted copy assignment .
     */
    constexpr static_vector_zero_storage& operator=(static_vector_zero_storage const&) = default;

    /**
     * @brief Defaulted move constructor.
     */
    constexpr static_vector_zero_storage(static_vector_zero_storage&&) = default;

    /**
     * @brief Defaulted move assignment.
     */
    constexpr static_vector_zero_storage& operator=(static_vector_zero_storage&&) = default;

    /**
     * @brief Defaulted destructor.
     */
    ~static_vector_zero_storage() = default;

    /**
     * @brief Pointer to the data in the storage.
     */
    static constexpr pointer data() noexcept { return nullptr; }

    /**
     * @brief Number of elements currently stored.
     */
    static constexpr size_type size() noexcept { return 0; }

    /**
     * @brief Capacity of the storage.
     */
    static constexpr size_type capacity() noexcept { return 0; }

    /**
     * @brief Is the storage empty?
     */
    static constexpr bool empty() noexcept { return true; }

    /**
     * @brief Is the storage full?
     */
    static constexpr bool full() noexcept { return true; }

    /**
     * @brief Constructs a new element at the end of the storagein-place. Increases size of the storage by one. Always
     * fails for empty storage.
     */
    template<typename... Args>
    static constexpr auto emplace_back(Args&&...) noexcept
        -> std::enable_if_t<std::is_constructible_v<T, Args...>, void>
    {
        assert(false && "tried to emplace_back on empty storage");
    }

    /**
     * @brief Removes the last element of the storage. Always fails for empty storage.
     */
    static constexpr void pop_back() noexcept { assert(false && "tried to pop_back on empty storage"); }

protected:
    /**
     * @brief Changes the size of the storage without adding or removing elements (unsafe). The size of an empty storage
     * can only be changed to 0.
     */
    static constexpr void unsafe_set_size(size_t new_size) noexcept
    {
        assert(new_size == 0
               && "tried to change size of empty storage to "
                  "non-zero value");
    }

    /**
     * @brief Destroys all elements of the storage in range [begin, end) without changings its size (unsafe). Nothing to
     * destroy since the storage is empty.
     */
    template<typename InputIt>
    static constexpr auto unsafe_destroy(InputIt /* begin */, InputIt /* end */) noexcept -> void
    {
    }

    /**
     * @brief Destroys all elements of the storage without changing its size (unsafe). Nothing to
     * destroy since the storage is empty.
     */
    static constexpr void unsafe_destroy_all() noexcept { }
};

/**
 * @brief Storage for trivial types.
 */
template<typename T, size_t Capacity>
class static_vector_trivial_storage
{
    static_assert(std::is_trivial_v<T>, "storage::trivial<T, C> requires std::is_trivial_v<T>");
    static_assert(Capacity != size_t {0}, "Capacity must be greater "
                                          "than zero (use "
                                          "storage::static_vector_zero_storage instead)");

public:
    using size_type       = smallest_size_t<Capacity>;
    using value_type      = T;
    using difference_type = ptrdiff_t;
    using pointer         = T*;
    using const_pointer   = T const*;

    constexpr static_vector_trivial_storage() noexcept                                     = default;
    constexpr static_vector_trivial_storage(static_vector_trivial_storage const&) noexcept = default;
    constexpr static_vector_trivial_storage& operator=(static_vector_trivial_storage const&) noexcept = default;
    constexpr static_vector_trivial_storage(static_vector_trivial_storage&&) noexcept                 = default;
    constexpr static_vector_trivial_storage& operator=(static_vector_trivial_storage&&) noexcept = default;
    ~static_vector_trivial_storage()                                                             = default;

    /**
     * @brief Direct access to the underlying storage.
     */
    [[nodiscard]] constexpr auto data() const noexcept -> const_pointer { return data_.data(); }

    /**
     * @brief Direct access to the underlying storage.
     */
    [[nodiscard]] constexpr auto data() noexcept -> pointer { return data_.data(); }

    /**
     * @brief Number of elements in the storage.
     */
    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return size_; }

    /**
     * @brief Maximum number of elements that can be allocated in the storage.
     */
    [[nodiscard]] constexpr auto capacity() noexcept -> size_type { return Capacity; }

    /**
     * @brief Is the storage empty?
     */
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == size_type {0}; }

    /**
     * @brief Is the storage full?
     */
    [[nodiscard]] constexpr auto full() const noexcept -> bool { return size() == Capacity; }

    /**
     * @brief Constructs an element in-place at the end of the storage.
     */
    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) noexcept
        -> std::enable_if_t<std::is_constructible_v<T, Args...> and std::is_assignable_v<value_type&, T>, void>
    {
        assert(!full() && "tried to emplace_back on full storage!");
        index(data_, size()) = T(std::forward<Args>(args)...);
        unsafe_set_size(size() + 1);
    }

    /**
     * @brief Remove the last element from the container.
     */
    constexpr auto pop_back() noexcept -> void
    {
        assert(!empty() && "tried to pop_back from empty storage!");
        unsafe_set_size(size() - 1);
    }

protected:
    /**
     * @brief (unsafe) Changes the container size to \p new_size.
     *
     * @warning No elements are constructed or destroyed.
     */
    constexpr auto unsafe_set_size(size_t new_size) noexcept -> void
    {
        assert(new_size <= Capacity && "new_size out-of-bounds [0, Capacity]");
        size_ = size_type(new_size);
    }

    /**
     * @brief (unsafe) Destroy elements in the range [begin, end).
     *
     * @warning The size of the storage is not changed.
     */
    template<typename InputIt>
    constexpr auto unsafe_destroy(InputIt, InputIt) noexcept -> void
    {
    }

    /**
     * @brief (unsafe) Destroys all elements of the storage.
     *
     * @warning The size of the storage is not changed.
     */
    constexpr auto unsafe_destroy_all() noexcept -> void { }

private:
    // If the value_type is const, make a const array of
    // non-const elements:
    using data_t = std::conditional_t<!std::is_const_v<T>, std::array<T, Capacity>,
                                      const std::array<std::remove_const_t<T>, Capacity>>;
    alignas(alignof(T)) data_t data_ {};

    size_type size_ = 0;
};

/**
 * @brief Storage for non-trivial elements.
 */
template<typename T, size_t Capacity>
class static_vector_non_trivial_storage
{
    static_assert(!std::is_trivial_v<T>, "use storage::trivial for std::is_trivial_v<T> elements");
    static_assert(Capacity != size_t {0}, "Capacity must be greater than zero!");

public:
    using size_type       = smallest_size_t<Capacity>;
    using value_type      = T;
    using difference_type = ptrdiff_t;
    using pointer         = T*;
    using const_pointer   = T const*;

    constexpr static_vector_non_trivial_storage()                                         = default;
    constexpr static_vector_non_trivial_storage(static_vector_non_trivial_storage const&) = default;
    constexpr static_vector_non_trivial_storage& operator=(static_vector_non_trivial_storage const&) = default;
    constexpr static_vector_non_trivial_storage(static_vector_non_trivial_storage&&)                 = default;
    constexpr static_vector_non_trivial_storage& operator=(static_vector_non_trivial_storage&&) = default;
    ~static_vector_non_trivial_storage() noexcept(std::is_nothrow_destructible_v<T>) { unsafe_destroy_all(); }

    /**
     * @brief Direct access to the underlying storage.
     */
    [[nodiscard]] auto data() const noexcept -> const_pointer { return reinterpret_cast<const_pointer>(data_); }

    /**
     * @brief Direct access to the underlying storage.
     */
    [[nodiscard]] auto data() noexcept -> pointer { return reinterpret_cast<pointer>(data_); }

    /**
     * @brief Pointer to one-past-the-end.
     */
    [[nodiscard]] auto end() const noexcept -> const_pointer { return data() + size(); }

    /**
     * @brief Pointer to one-past-the-end.
     */
    [[nodiscard]] auto end() noexcept -> pointer { return data() + size(); }

    /**
     * @brief Number of elements in the storage.
     */
    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return size_; }

    /**
     * @brief Maximum number of elements that can be allocated in the storage.
     */
    [[nodiscard]] constexpr auto capacity() noexcept -> size_type { return Capacity; }

    /**
     * @brief Is the storage empty?
     */
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == size_type {0}; }

    /**
     * @brief Is the storage full?
     */
    [[nodiscard]] constexpr auto full() const noexcept -> bool { return size() == Capacity; }

    /**
     * @brief Constructs an element in-place at the end of the embedded storage.
     */
    template<typename... Args>
    auto emplace_back(Args&&... args) noexcept(noexcept(new (end()) T(std::forward<Args>(args)...)))
        -> std::enable_if_t<std::is_copy_constructible_v<T>, void>
    {
        assert(!full() && "tried to emplace_back on full storage");
        new (end()) T(std::forward<Args>(args)...);
        unsafe_set_size(size() + 1);
    }

    /**
     * @brief Remove the last element from the container.
     */
    auto pop_back() noexcept(std::is_nothrow_destructible_v<T>) -> void
    {
        assert(!empty() && "tried to pop_back from empty storage!");
        auto ptr = end() - 1;
        ptr->~T();
        unsafe_set_size(size() - 1);
    }

protected:
    /**
     * @brief (unsafe) Changes the container size to \p new_size.
     *
     * @warning No elements are constructed or destroyed.
     */
    constexpr void unsafe_set_size(size_t new_size) noexcept
    {
        assert(new_size <= Capacity && "new_size out-of-bounds [0, Capacity)");
        size_ = size_type(new_size);
    }

    /**
     * @brief (unsafe) Destroy elements in the range [begin, end).
     *
     * @warning The size of the storage is not changed.
     */
    template<typename InputIt>
    void unsafe_destroy(InputIt first, InputIt last) noexcept(std::is_nothrow_destructible_v<T>)
    {
        assert(first >= data() && first <= end() && "first is out-of-bounds");
        assert(last >= data() && last <= end() && "last is out-of-bounds");
        for (; first != last; ++first)
        {
            first->~T();
        }
    }

    /**
     * @brief (unsafe) Destroys all elements of the storage.
     *
     * @warning The size of the storage is not changed.
     */
    void unsafe_destroy_all() noexcept(std::is_nothrow_destructible_v<T>) { unsafe_destroy(data(), end()); }

private:
    using raw_type          = std::remove_const_t<T>;
    using aligned_storage_t = std::aligned_storage_t<sizeof(raw_type), alignof(raw_type)>;
    using storage_type      = std::conditional_t<!std::is_const_v<T>, aligned_storage_t, const aligned_storage_t>;

    alignas(alignof(T)) storage_type data_[Capacity] {};
    size_type size_ = 0;
};

/**
 * @brief Selects the vector storage.
 */
template<typename T, size_t Capacity>
using storage_type
    = std::conditional_t<Capacity == 0, static_vector_zero_storage<T>,
                         std::conditional_t<std::is_trivial_v<T>, static_vector_trivial_storage<T, Capacity>,
                                            static_vector_non_trivial_storage<T, Capacity>>>;

}  // namespace detail

/**
 * @brief Dynamically-resizable fixed-capacity vector.
 */
template<typename T, size_t Capacity>
class static_vector : private detail::storage_type<T, Capacity>
{
private:
    static_assert(std::is_nothrow_destructible_v<T>, "T must be nothrow destructible");
    using base_type = detail::storage_type<T, Capacity>;
    using self      = static_vector<T, Capacity>;

    using base_type::unsafe_destroy;
    using base_type::unsafe_destroy_all;
    using base_type::unsafe_set_size;

public:
    using value_type             = typename base_type::value_type;
    using difference_type        = ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = typename base_type::pointer;
    using const_pointer          = typename base_type::const_pointer;
    using iterator               = typename base_type::pointer;
    using const_iterator         = typename base_type::const_pointer;
    using size_type              = size_t;
    using reverse_iterator       = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

    /**
     * @brief Default constructor.
     */
    constexpr static_vector() = default;

    /**
     * @brief Copy constructor.
     */
    constexpr static_vector(static_vector const& other) noexcept(noexcept(this->insert(this->begin(), other.begin(),
                                                                                       other.end())))
    {
        // Nothing to assert: size of other cannot exceed capacity because both vectors have the same type
        insert(this->begin(), other.begin(), other.end());
    }

    /**
     * @brief Move constructor.
     */
    constexpr static_vector(static_vector&& other) noexcept(noexcept(this->move_insert(this->begin(), other.begin(),
                                                                                       other.end())))
    {
        // Nothing to assert: size of other cannot exceed capacity because both vectors have the same type
        move_insert(this->begin(), other.begin(), other.end());
    }

    /**
     * @brief Copy assignment.
     */
    constexpr auto operator=(static_vector const& other) noexcept(
        noexcept(this->clear()) && noexcept(this->insert(this->begin(), other.begin(), other.end())))
        -> std::enable_if_t<std::is_assignable_v<reference, const_reference>, static_vector&>
    {
        // Nothing to assert: size of other cannot exceed capacity because both vectors have the same type
        clear();
        insert(this->begin(), other.begin(), other.end());
        return *this;
    }

    /**
     * @brief Move assignment.
     */
    constexpr auto operator=(static_vector&& other) noexcept(
        noexcept(this->clear()) and noexcept(this->move_insert(this->begin(), other.begin(), other.end())))
        -> std::enable_if_t<std::is_assignable_v<reference, reference>, static_vector&>
    {
        // Nothing to assert: size of other cannot exceed capacity because both vectors have the same type
        clear();
        move_insert(this->begin(), other.begin(), other.end());
        return *this;
    }

    /**
     * @brief Initializes vector with \p n default-constructed elements.
     *
     * @todo FCV_REQUIRES(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>)
     */
    explicit constexpr static_vector(size_type n) noexcept(noexcept(this->emplace_n(n)))
    {
        assert(n <= capacity() && "size exceeds capacity");
        emplace_n(n);
    }

    /**
     * @brief Initializes vector with \p n with \p value.
     *
     * @todo FCV_REQUIRES(std::is_copy_constructible_v<T>)
     */
    constexpr static_vector(size_type n, T const& value) noexcept(noexcept(this->insert(this->begin(), n, value)))
    {
        assert(n <= capacity() && "size exceeds capacity");
        this->insert(this->begin(), n, value);
    }

    /**
     * @brief Initialize vector from range [first, last).
     *
     * @todo FCV_REQUIRES_(detail::InputIterator<InputIt>)
     */
    template<class InputIter>
    constexpr static_vector(InputIter first, InputIter last)
    {
        if constexpr (detail::RandomAccessIterator<InputIter>)
        {
            assert(last - first >= 0);
            assert(static_cast<size_type>(last - first) <= capacity() && "range size exceeds capacity");
        }
        insert(this->begin(), first, last);
    }

    [[nodiscard]] constexpr auto begin() noexcept -> iterator { return data(); }
    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator { return data(); }
    [[nodiscard]] constexpr auto end() noexcept -> iterator { return data() + size(); }
    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator { return data() + size(); }

    [[nodiscard]] auto rbegin() noexcept -> reverse_iterator { return reverse_iterator(end()); }
    [[nodiscard]] auto rbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator(end()); }
    [[nodiscard]] auto rend() noexcept -> reverse_iterator { return reverse_iterator(this->begin()); }
    [[nodiscard]] auto rend() const noexcept -> const_reverse_iterator { return const_reverse_iterator(this->begin()); }

    [[nodiscard]] constexpr auto cbegin() noexcept -> const_iterator { return begin(); }
    [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }
    [[nodiscard]] constexpr auto cend() noexcept -> const_iterator { return end(); }
    [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator { return end(); }

    /**
     * @brief Is the storage empty?
     */
    using base_type::empty;

    /**
     * @brief Is the storage full?
     */
    using base_type::full;

    /**
     * @brief Number of elements in the vector
     */
    constexpr size_type size() const noexcept { return base_type::size(); }

    /**
     * @brief Maximum number of elements that can be allocated in the vector
     */
    constexpr size_type capacity() noexcept { return base_type::capacity(); }

    /**
     * @brief Maximum number of elements that can be allocated in the vector
     */
    constexpr size_type max_size() noexcept { return capacity(); }

    /**
     * @brief Data access
     */
    using base_type::data;

    /**
     * @brief Resizes the container to contain \p sz elements. If elements need to be appended, these are
     * move-constructed from `T{}` (or copy-constructed if `T` is not `std::is_move_constructible_v`).
     */
    constexpr auto
    resize(size_type sz) noexcept((std::is_move_constructible_v<T> && std::is_nothrow_move_constructible_v<T>)
                                  || (std::is_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>))
        -> std::enable_if_t<detail::is_movable_v<value_type>, void>
    {
        if (sz == size())
        {
            return;
        }

        if (sz > size())
        {
            emplace_n(sz);
        }
        else
        {
            erase(end() - (size() - sz), end());
        }
    }

    /**
     * @brief Clears the vector.
     */
    constexpr void clear() noexcept
    {
        unsafe_destroy_all();
        unsafe_set_size(0);
    }

    /**
     * @brief
     */
    template<class InputIter>
    constexpr auto
    assign(InputIter first,
           InputIter last) noexcept(noexcept(this->clear()) and noexcept(this->insert(this->begin(), first, last)))
        -> std::enable_if_t<detail::InputIterator<InputIter>, void>
    {
        if constexpr (detail::RandomAccessIterator<InputIter>)
        {
            assert(last - first >= 0);
            assert(static_cast<size_type>(last - first) <= capacity() && "range size exceeds capacity");
        }
        clear();
        insert(this->begin(), first, last);
    }

    /**
     * @brief
     */
    constexpr auto assign(size_type n, const T& u) -> std::enable_if_t<std::is_copy_constructible_v<T>, void>
    {
        assert(n <= capacity() && "size exceeds capacity");
        clear();
        insert(this->begin(), n, u);
    }

    /**
     * @brief Unchecked access to element at index \p pos (UB if index not in range)
     */
    [[nodiscard]] constexpr auto operator[](size_type pos) noexcept -> reference { return detail::index(*this, pos); }

    /**
     * @brief Unchecked access to element at index \p pos (UB if index not in range)
     */
    [[nodiscard]] constexpr auto operator[](size_type pos) const noexcept -> const_reference
    {
        return detail::index(*this, pos);
    }

    /**
     * @brief
     */
    [[nodiscard]] constexpr auto front() noexcept -> reference { return detail::index(*this, 0); }

    /**
     * @brief
     */
    [[nodiscard]] constexpr auto front() const noexcept -> const_reference { return detail::index(*this, 0); }

    /**
     * @brief
     */
    [[nodiscard]] constexpr auto back() noexcept -> reference
    {
        assert(!empty() && "calling back on an empty vector");
        return detail::index(*this, size() - 1);
    }

    /**
     * @brief
     */
    [[nodiscard]] constexpr auto back() const noexcept -> const_reference
    {
        assert(!empty() && "calling back on an empty vector");
        return detail::index(*this, size() - 1);
    }

    /**
     * @brief
     */
    using base_type::emplace_back;

    /**
     * @brief
     */
    using base_type::pop_back;

    /**
     * @brief Appends \p value at the end of the vector.
     */
    template<typename U>
    constexpr auto push_back(U&& value) noexcept(noexcept(emplace_back(std::forward<U>(value))))
        -> std::enable_if_t<std::is_constructible_v<T, U> && std::is_assignable_v<reference, U&&>, void>
    {
        assert(!full() && "vector is full!");
        emplace_back(std::forward<U>(value));
    }

    /**
     * @brief
     */
    template<typename... Args>
    constexpr auto emplace(const_iterator position,
                           Args&&... args) noexcept(noexcept(this->move_insert(position, std::declval<value_type*>(),
                                                                               std::declval<value_type*>())))
        -> std::enable_if_t<std::is_constructible_v<T, Args...>, iterator>
    {
        assert(!full() && "tried emplace on full static_vector!");
        assert_iterator_in_range(position);
        value_type a(std::forward<Args>(args)...);
        return move_insert(position, &a, &a + 1);
    }

    /**
     * @brief
     */
    constexpr auto insert(const_iterator position,
                          const_reference x) noexcept(noexcept(this->insert(position, size_type(1), x)))
        -> std::enable_if_t<std::is_copy_constructible_v<T>, iterator>
    {
        assert(!full() && "tried insert on full static_vector!");
        assert_iterator_in_range(position);
        return insert(position, size_type(1), x);
    }

    /**
     * @brief
     */
    constexpr auto insert(const_iterator position,
                          value_type&& x) noexcept(noexcept(this->move_insert(position, &x, &x + 1)))
        -> std::enable_if_t<std::is_move_constructible_v<T>, iterator>
    {
        assert(!full() && "tried insert on full static_vector!");
        assert_iterator_in_range(position);
        return move_insert(position, &x, &x + 1);
    }

    /**
     * @brief
     */
    constexpr auto insert(const_iterator position, size_type n, const T& x) noexcept(noexcept(push_back(x)))
        -> std::enable_if_t<std::is_copy_constructible_v<T>, iterator>
    {
        assert_iterator_in_range(position);
        const auto new_size = size() + n;
        assert(new_size <= capacity() && "trying to insert beyond capacity!");
        auto b = end();
        while (n != 0)
        {
            push_back(x);
            --n;
        }

        auto writable_position = begin() + (position - begin());
        std::rotate(writable_position, b, end());
        return writable_position;
    }

    /**
     * @brief detail::InputIterator<InputIt>and std::is_constructible_v<value_type,
     * detail::iterator_reference_t<InputIt>>
     */
    template<class InputIt>
    constexpr iterator insert(const_iterator position, InputIt first,
                              InputIt last) noexcept(noexcept(emplace_back(*first)))
    {
        assert_iterator_in_range(position);
        assert_valid_iterator_pair(first, last);
        if constexpr (detail::RandomAccessIterator<InputIt>)
        {
            assert(size() + static_cast<size_type>(last - first) <= capacity() && "trying to insert beyond capacity!");
        }
        auto b = end();

        // insert at the end and then just rotate:
        for (; first != last; ++first)
        {
            emplace_back(*first);
        }

        auto writable_position = begin() + (position - begin());
        std::rotate(writable_position, b, end());
        return writable_position;
    }

    /**
     * @brief FCV_REQUIRES_(detail::InputIterator<InputIt>)
     */
    template<class InputIt>
    constexpr iterator move_insert(const_iterator position, InputIt first,
                                   InputIt last) noexcept(noexcept(emplace_back(std::move(*first))))
    {
        assert_iterator_in_range(position);
        assert_valid_iterator_pair(first, last);
        if constexpr (detail::RandomAccessIterator<InputIt>)
        {
            assert(size() + static_cast<size_type>(last - first) <= capacity() && "trying to insert beyond capacity!");
        }
        iterator b = end();

        // we insert at the end and then just rotate:
        for (; first != last; ++first)
        {
            emplace_back(std::move(*first));
        }
        auto writable_position = begin() + (position - begin());
        std::rotate<iterator>(writable_position, b, end());
        return writable_position;
    }

    /**
     * @brief
     */
    constexpr auto erase(const_iterator position) noexcept
        -> std::enable_if_t<detail::is_movable_v<value_type>, iterator>
    {
        assert_iterator_in_range(position);
        return erase(position, position + 1);
    }

    /**
     * @brief
     */
    constexpr auto erase(const_iterator first, const_iterator last) noexcept
        -> std::enable_if_t<detail::is_movable_v<value_type>, iterator>
    {
        assert_iterator_pair_in_range(first, last);
        iterator p = begin() + (first - begin());
        if (first != last)
        {
            unsafe_destroy(std::move(p + (last - first), end(), p), end());
            unsafe_set_size(size() - static_cast<size_type>(last - first));
        }

        return p;
    }

    /**
     * @brief
     */
    constexpr auto swap(static_vector& other) noexcept(std::is_nothrow_swappable_v<T>)
        -> std::enable_if_t<std::is_assignable_v<T&, T&&>, void>
    {
        static_vector tmp = std::move(other);
        other             = std::move(*this);
        (*this)           = std::move(tmp);
    }

    /**
     * @brief Resizes the container to contain \p sz elements. If elements need to be appended, these are
     * copy-constructed from \p value.
     */
    constexpr auto resize(size_type sz, T const& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
        -> std::enable_if_t<std::is_copy_constructible_v<T>, void>
    {
        if (sz == size())
        {
            return;
        }
        if (sz > size())
        {
            assert(sz <= capacity()
                   && "static_vector cannot be resized to "
                      "a size greater than capacity");
            insert(end(), sz - size(), value);
        }
        else
        {
            erase(end() - (size() - sz), end());
        }
    }

private:
    template<typename It>
    constexpr void assert_iterator_in_range(It it) noexcept
    {
        static_assert(std::is_pointer_v<It>);
        assert(this->begin() <= it && "iterator not in range");
        assert(it <= end() && "iterator not in range");
    }

    template<typename It0, typename It1>
    constexpr void assert_valid_iterator_pair(It0 first, It1 last) noexcept
    {
        static_assert(std::is_pointer_v<It0>);
        static_assert(std::is_pointer_v<It1>);
        assert(first <= last && "invalid iterator pair");
    }

    template<typename It0, typename It1>
    constexpr void assert_iterator_pair_in_range(It0 first, It1 last) noexcept
    {
        assert_iterator_in_range(first);
        assert_iterator_in_range(last);
        assert_valid_iterator_pair(first, last);
    }

    constexpr auto
    emplace_n(size_type n) noexcept((std::is_move_constructible_v<T> && std::is_nothrow_move_constructible_v<T>)
                                    || (std::is_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>))
        -> std::enable_if_t<std::is_move_constructible_v<T> or std::is_copy_constructible_v<T>, void>
    {
        assert(n <= capacity()
               && "static_vector cannot be "
                  "resized to a size greater than "
                  "capacity");
        while (n != size())
        {
            emplace_back(T {});
        }
    }
};

/**
 * @brief
 */
template<typename T, size_t Capacity>
constexpr auto operator==(static_vector<T, Capacity> const& lhs, static_vector<T, Capacity> const& rhs) noexcept -> bool
{
    if (std::size(lhs) == std::size(rhs))
    {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), std::equal_to<> {});
    }
    return false;
}

/**
 * @brief
 */
template<typename T, size_t Capacity>
constexpr auto operator<(static_vector<T, Capacity> const& lhs, static_vector<T, Capacity> const& rhs) noexcept -> bool
{
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), std::less<> {});
}

/**
 * @brief
 */
template<typename T, size_t Capacity>
constexpr auto operator!=(static_vector<T, Capacity> const& lhs, static_vector<T, Capacity> const& rhs) noexcept -> bool
{
    return not(lhs == rhs);
}

/**
 * @brief
 */
template<typename T, size_t Capacity>
constexpr auto operator<=(static_vector<T, Capacity> const& lhs, static_vector<T, Capacity> const& rhs) noexcept -> bool
{
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), std::less_equal<> {});
}

/**
 * @brief
 */
template<typename T, size_t Capacity>
constexpr auto operator>(static_vector<T, Capacity> const& lhs, static_vector<T, Capacity> const& rhs) noexcept -> bool
{
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), std::greater<> {});
}

/**
 * @brief
 */
template<typename T, size_t Capacity>
constexpr auto operator>=(static_vector<T, Capacity> const& lhs, static_vector<T, Capacity> const& rhs) noexcept -> bool
{
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), std::greater_equal<> {});
}

}  // namespace etl