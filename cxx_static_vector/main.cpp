#include "static_vector.hpp"

#include <numeric>

#define ASSERT(x)                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            std::printf("ASSERTION: %s:%d\n", __FILE__, __LINE__);                                                     \
            std::exit(EXIT_FAILURE);                                                                                   \
        }                                                                                                              \
    } while (false)

#define ASSERT_FALSE(x) ASSERT(!(x))

template<typename T>
auto test_builtin_type()
{
    // construct zero sized
    etl::static_vector<T, 0> zero {};
    ASSERT(zero.empty());
    ASSERT(zero.size() == 0);
    ASSERT(zero.capacity() == 0);
    ASSERT(zero.data() == nullptr);
    ASSERT(zero.full());

    // construct empty
    etl::static_vector<T, 16> lhs {};
    ASSERT(lhs.empty());
    ASSERT(lhs.size() == 0);
    ASSERT_FALSE(lhs.full());

    ASSERT(std::begin(lhs) == std::end(lhs));
    ASSERT(std::cbegin(lhs) == std::cend(lhs));
    ASSERT(std::begin(std::as_const(lhs)) == std::end(std::as_const(lhs)));

    etl::static_vector<T, 16> rhs {};
    ASSERT(rhs.empty());
    ASSERT(rhs.size() == 0);
    ASSERT_FALSE(rhs.full());

    // comparison empty
    ASSERT(lhs == rhs);
    ASSERT(rhs == lhs);
    ASSERT_FALSE(lhs != rhs);
    ASSERT_FALSE(rhs != lhs);

    // push_back
    rhs.push_back(T {1});
    ASSERT_FALSE(rhs.empty());
    ASSERT_FALSE(rhs == lhs);
    ASSERT(rhs.size() == T {1});

    lhs.push_back(T {1});
    ASSERT_FALSE(lhs.empty());
    ASSERT(rhs == lhs);
    ASSERT(lhs.size() == T {1});

    // swap
    lhs.swap(rhs);
    ASSERT(lhs == rhs);
    rhs.swap(lhs);
    ASSERT(lhs == rhs);

    // resize
    auto vec_0 = etl::static_vector<T, 4> {4};
    ASSERT(vec_0.size() == 4);
    ASSERT(std::all_of(std::begin(vec_0), std::end(vec_0), [](auto val) { return val == T(); }));
    ASSERT(std::all_of(std::cbegin(vec_0), std::cend(vec_0), [](auto val) { return val == T(); }));

    // construct size
    etl::static_vector<T, 16> vec_01(8);
    ASSERT(vec_01.size() == 8);
    ASSERT(std::all_of(std::cbegin(vec_0), std::cend(vec_0), [](auto val) { return val == T(); }));

    // construct begin(), end()
    auto vec_1 = etl::static_vector<T, 16> {std::begin(lhs), std::end(lhs)};
    ASSERT(lhs == vec_1);
    ASSERT(rhs == vec_1);

    // copy consrtuct
    etl::static_vector<T, 16> vec_2 {vec_1};
    ASSERT(vec_1 == vec_2);
    ASSERT_FALSE(vec_1 != vec_2);

    // copy assignment
    etl::static_vector<T, 16> vec_3 {};
    vec_3 = vec_1;
    ASSERT(vec_1 == vec_3);
    ASSERT_FALSE(vec_1 != vec_3);

    // move construct
    auto const vec_4 {std::move(vec_1)};
    ASSERT(vec_3 == vec_4);
    ASSERT_FALSE(vec_3 != vec_4);

    // move assignment
    auto const vec_5 = std::move(vec_2);
    ASSERT(vec_3 == vec_5);
    ASSERT_FALSE(vec_3 != vec_5);
    ASSERT(vec_5.front() == T(1));
    ASSERT(vec_5.back() == T(1));

    // resize
    auto vec_6 = etl::static_vector<T, 4> {};
    ASSERT(vec_6.size() == 0);
    // grow
    vec_6.resize(2);
    ASSERT(vec_6.size() == 2);
    ASSERT(std::all_of(vec_6.begin(), vec_6.end(), [](auto val) { return val == T(); }));
    // grow full capacity
    vec_6.resize(4, T(0));
    ASSERT(vec_6.size() == 4);
    ASSERT(std::all_of(vec_6.cbegin(), vec_6.cend(), [](auto val) { return val == T(); }));
    // same size
    vec_6.resize(4, T(0));
    ASSERT(vec_6.size() == 4);
    ASSERT(std::all_of(vec_6.begin(), vec_6.end(), [](auto val) { return val == T(); }));
    // shrink
    vec_6.resize(2, T(1));
    ASSERT(vec_6.size() == 2);
    // ASSERT(std::all_of(vec_6.cbegin(), vec_6.cend(), [](auto val) { return val == T(1); }));

    // pop_back
    auto vec_7 = etl::static_vector<T, 4> {};
    ASSERT(vec_7.size() == 0);
    vec_7.push_back(1);
    ASSERT(vec_7.size() == 1);
    vec_7.pop_back();
    ASSERT(vec_7.size() == 0);

    // earse
    auto vec_8 = etl::static_vector<T, 4> {4};
    std::generate(std::begin(vec_8), std::end(vec_8), [] {
        static auto val = T {};
        return val += T(1);
    });

    ASSERT(vec_8.front() == T(1));
    vec_8.erase(vec_8.begin());
    ASSERT(vec_8.front() == T(2));

    // assign
    auto vec_9 = etl::static_vector<T, 4> {};
    vec_9.assign(4, T {42});
    ASSERT(vec_9.size() == 4);
    ASSERT(vec_9.front() == 42);
    ASSERT(vec_9.back() == 42);
    auto const& cvec_9 = std::as_const(vec_9);
    ASSERT(std::all_of(cvec_9.cbegin(), cvec_9.cend(), [](auto val) { return val == T(42); }));

    auto vec_10 = etl::static_vector<T, 4> {4};
    vec_10.assign(vec_9.begin(), vec_9.end());
    ASSERT(vec_10.size() == 4);
    ASSERT(vec_10.front() == 42);
    ASSERT(vec_10.back() == 42);
    auto const& cvec_10 = std::as_const(vec_10);
    ASSERT(std::all_of(cvec_10.cbegin(), cvec_10.cend(), [](auto val) { return val == T(42); }));
}

template<typename T>
struct Vertex
{
    T x;
    T y;
    T z;

    Vertex(T x, T y, T z) : x {x}, y {y}, z {z} { }
};

template<typename T>
[[nodiscard]] constexpr auto operator==(Vertex<T> const& lhs, Vertex<T> const& rhs) -> bool
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<typename T>
[[nodiscard]] constexpr auto operator!=(Vertex<T> const& lhs, Vertex<T> const& rhs) -> bool
{
    return !(lhs == rhs);
}

template<typename T>
auto test_struct()
{
    // construct zero sized
    etl::static_vector<Vertex<T>, 0> zero {};
    ASSERT(zero.empty());
    ASSERT(zero.size() == 0);
    ASSERT(zero.capacity() == 0);
    ASSERT(zero.data() == nullptr);
    ASSERT(zero.full());

    // construct empty
    etl::static_vector<Vertex<T>, 16> lhs {};
    ASSERT(lhs.empty());
    etl::static_vector<Vertex<T>, 16> rhs {};
    ASSERT(rhs.empty());

    ASSERT(std::begin(lhs) == std::end(lhs));
    ASSERT(std::cbegin(lhs) == std::cend(lhs));
    ASSERT(std::begin(std::as_const(lhs)) == std::end(std::as_const(lhs)));

    // compare empty
    ASSERT(lhs == rhs);
    ASSERT(rhs == lhs);
    ASSERT_FALSE(lhs != rhs);
    ASSERT_FALSE(rhs != lhs);

    // emplace_back
    rhs.emplace_back(T(1.20f), T(1.00f), T(1.43f));
    ASSERT_FALSE(rhs.empty());
    ASSERT_FALSE(rhs == lhs);
    ASSERT(rhs.size() == 1);

    lhs.emplace_back(T(1.20f), T(1.00f), T(1.43f));
    ASSERT_FALSE(lhs.empty());
    ASSERT(rhs == lhs);
    ASSERT(lhs.size() == 1);

    // construct copy
    {
        etl::static_vector<Vertex<T>, 3> original {3, Vertex {T(1), T(2), T(3)}};
        auto copy {original};
        ASSERT(copy.full());
        ASSERT(original == copy);
    }

    // assignment copy
    {
        etl::static_vector<Vertex<T>, 3> original {3, Vertex {T(1), T(2), T(3)}};
        auto copy = original;
        ASSERT(copy.full());
        ASSERT(original == copy);
    }

    // pop_back
    {
        etl::static_vector<Vertex<T>, 3> original {};
        auto vertex = Vertex {T(1), T(2), T(3)};
        original.push_back(vertex);
        ASSERT(original.size() == 1);
        ASSERT(original.front() == vertex);
        ASSERT(original.back() == vertex);

        original.pop_back();
        ASSERT(original.size() == 0);
    }

    // emplace
    {
        etl::static_vector<Vertex<T>, 3> vec {};
        vec.emplace(vec.end(), T(1.20f), T(1.00f), T(1.43f));
        ASSERT_FALSE(vec.empty());
        ASSERT(rhs.size() == 1);
    }

    // insert copy
    {
        auto vec = etl::static_vector<Vertex<T>, 3> {};
        ASSERT(vec.size() == 0);
        auto vertex = Vertex {T(1.20f), T(1.00f), T(1.43f)};
        vec.insert(vec.begin(), vertex);
        vec.insert(vec.begin(), vertex);
        ASSERT(vec.size() == 2);
    }

    // insert move
    {
        auto vec = etl::static_vector<Vertex<T>, 3> {};
        ASSERT(vec.size() == 0);
        vec.insert(vec.begin(), Vertex {T(1.20f), T(1.00f), T(1.43f)});
        vec.insert(vec.begin(), Vertex {T(1.20f), T(1.00f), T(1.43f)});
        ASSERT(vec.size() == 2);
    }

    // operator[]
    {
        auto a   = Vertex {T(1), T(1), T(1)};
        auto b   = Vertex {T(2), T(2), T(2)};
        auto vec = etl::static_vector<Vertex<T>, 3> {};
        ASSERT(vec.size() == 0);

        vec.insert(vec.begin(), a);
        vec.insert(vec.begin(), b);
        ASSERT(vec[0] == b);
        ASSERT(std::as_const(vec)[1] == a);
    }
}

int main()
{
    test_builtin_type<signed char>();
    test_builtin_type<signed short>();
    test_builtin_type<signed int>();
    test_builtin_type<signed long>();
    test_builtin_type<signed long long>();

    test_builtin_type<unsigned char>();
    test_builtin_type<unsigned short>();
    test_builtin_type<unsigned int>();
    test_builtin_type<unsigned long>();
    test_builtin_type<unsigned long long>();

    test_builtin_type<float>();
    test_builtin_type<double>();
    test_builtin_type<long double>();

    test_struct<signed char>();
    test_struct<signed short>();
    test_struct<signed int>();
    test_struct<signed long>();
    test_struct<signed long long>();

    test_struct<unsigned char>();
    test_struct<unsigned short>();
    test_struct<unsigned int>();
    test_struct<unsigned long>();
    test_struct<unsigned long long>();

    test_struct<float>();
    test_struct<double>();
    test_struct<long double>();

    return EXIT_SUCCESS;
}