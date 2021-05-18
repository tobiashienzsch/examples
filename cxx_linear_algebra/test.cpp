#include "matrix.hpp"
#include "vector.hpp"

#define REQUIRE(exp)                                                           \
    do                                                                         \
    {                                                                          \
        if (!(exp))                                                            \
        {                                                                      \
            ::std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << '\n';    \
            ::std::exit(EXIT_FAILURE);                                         \
        }                                                                      \
    } while (false)

template<typename T>
auto vector_test() -> void
{
    auto vec = ta::DynamicVector<T> {2};
    REQUIRE(vec.size() == 2);

    vec.resize(3);
    REQUIRE(vec.size() == 3);

    REQUIRE(vec.at(0) == T {});
    REQUIRE(vec.at(1) == T {});
    REQUIRE(vec.at(2) == T {});

    REQUIRE(vec[0] == T {});
    REQUIRE(vec[1] == T {});
    REQUIRE(vec[2] == T {});

    REQUIRE(vec == vec);
    REQUIRE(!(vec != vec));

    vec[0] = T {1};
    vec[1] = T {2};
    vec[2] = T {3};
    REQUIRE(vec[0] == T {1});
    REQUIRE(vec[1] == T {2});
    REQUIRE(vec[2] == T {3});

    auto other_size = ta::DynamicVector<T> {};
    REQUIRE(other_size.size() == 0);
    REQUIRE(vec != other_size);
    REQUIRE(!(vec == other_size));

    auto same_size = ta::DynamicVector<T> {3};
    REQUIRE(vec != same_size);
    REQUIRE(!(vec == same_size));

    vec.clear();
    REQUIRE(vec == same_size);
    REQUIRE(!(vec != same_size));

    auto zero_add = vec + same_size;
    REQUIRE(zero_add.size() == 3);
    REQUIRE(zero_add[0] == T {});
    REQUIRE(zero_add[1] == T {});
    REQUIRE(zero_add[2] == T {});

    auto zero_sub = vec - same_size;
    REQUIRE(zero_sub.size() == 3);
    REQUIRE(zero_sub[0] == T {});
    REQUIRE(zero_sub[1] == T {});
    REQUIRE(zero_sub[2] == T {});

    vec[0]             = T {1};
    vec[1]             = T {2};
    vec[2]             = T {3};
    auto half_zero_add = vec + same_size;
    REQUIRE(half_zero_add.size() == 3);
    REQUIRE(half_zero_add[0] == T {1});
    REQUIRE(half_zero_add[1] == T {2});
    REQUIRE(half_zero_add[2] == T {3});

    auto half_zero_sub = vec - same_size;
    REQUIRE(half_zero_sub.size() == 3);
    REQUIRE(half_zero_sub[0] == T {1});
    REQUIRE(half_zero_sub[1] == T {2});
    REQUIRE(half_zero_sub[2] == T {3});

    same_size[0] = T {1};
    same_size[1] = T {1};
    same_size[2] = T {1};
    auto add     = vec + same_size;
    REQUIRE(add.size() == 3);
    REQUIRE(add[0] == T {2});
    REQUIRE(add[1] == T {3});
    REQUIRE(add[2] == T {4});

    auto sub = vec - same_size;
    REQUIRE(sub.size() == 3);
    REQUIRE(sub[0] == T {0});
    REQUIRE(sub[1] == T {1});
    REQUIRE(sub[2] == T {2});

    auto scaled2 = vec * T {2};
    REQUIRE(scaled2.size() == 3);
    REQUIRE(scaled2[0] == T {2});
    REQUIRE(scaled2[1] == T {4});
    REQUIRE(scaled2[2] == T {6});

    auto scaled4 = T {4} * vec;
    REQUIRE(scaled4.size() == 3);
    REQUIRE(scaled4[0] == T {4});
    REQUIRE(scaled4[1] == T {8});
    REQUIRE(scaled4[2] == T {12});
}

auto main() -> int
{
    vector_test<int>();
    vector_test<float>();
    vector_test<double>();

    return EXIT_SUCCESS;
}