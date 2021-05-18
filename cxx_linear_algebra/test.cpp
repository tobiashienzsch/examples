#include "matrix.hpp"
#include "vector.hpp"

#include <cstring>
#include <sstream>
#include <utility>

#define REQUIRE(exp)                                                           \
    do                                                                         \
    {                                                                          \
        if (!(exp))                                                            \
        {                                                                      \
            ::std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__             \
                        << " in: " << __PRETTY_FUNCTION__ << '\n';             \
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

    vec[0] = T {1};
    vec[1] = T {2};
    vec[2] = T {3};

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

    auto add = vec + same_size;
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

    try
    {
        auto fail = vec + ta::DynamicVector<T> {5};
        REQUIRE(false);
    }
    catch (std::logic_error const& e)
    {
        auto const* msg = "Both DynamicVectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto fail = vec - ta::DynamicVector<T> {5};
        REQUIRE(false);
    }
    catch (std::logic_error const& e)
    {
        auto const* msg = "Both DynamicVectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto lhs = ta::DynamicVector<T> {2};
    auto rhs = ta::DynamicVector<T> {2};
    lhs[0]   = T {1};
    lhs[1]   = T {2};
    rhs[0]   = T {1};
    rhs[1]   = T {2};
    REQUIRE(ta::dotProduct(lhs, rhs) == T {5});

    rhs[0] = T {4};
    rhs[1] = T {4};
    REQUIRE(ta::dotProduct(lhs, rhs) == T {12});

    try
    {
        ta::dotProduct(lhs, ta::DynamicVector<T> {3});
        REQUIRE(false);
    }
    catch (std::logic_error const& e)
    {
        auto const* msg = "Both DynamicVectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto stream = std::stringstream {};
    stream << lhs;
    REQUIRE(stream.str() == "1 2 ");
}

template<typename T>
auto matrix_test() -> void
{
    auto mat = ta::DynamicMatrix<T> {};
    REQUIRE(mat.rows() == 0);
    REQUIRE(mat.cols() == 0);
    REQUIRE(mat.size() == 0);

    mat.resize(2, 2);
    REQUIRE(mat.rows() == 2);
    REQUIRE(mat.cols() == 2);
    REQUIRE(mat.size() == 4);

    REQUIRE(mat.at(0, 0) == T {0});
    REQUIRE(mat.at(0, 1) == T {0});
    REQUIRE(mat.at(1, 0) == T {0});
    REQUIRE(mat.at(1, 1) == T {0});

    mat.at(0, 0) = T {0};
    mat.at(0, 1) = T {1};
    mat.at(1, 0) = T {2};
    mat.at(1, 1) = T {3};

    auto const& const_mat = mat;
    REQUIRE(const_mat.at(0, 0) == T {0});
    REQUIRE(const_mat.at(0, 1) == T {1});
    REQUIRE(const_mat.at(1, 0) == T {2});
    REQUIRE(const_mat.at(1, 1) == T {3});

    auto scaled = mat * T {2};
    REQUIRE(scaled.at(0, 0) == T {0});
    REQUIRE(scaled.at(0, 1) == T {2});
    REQUIRE(scaled.at(1, 0) == T {4});
    REQUIRE(scaled.at(1, 1) == T {6});

    auto scaled_twice = T {2} * scaled;
    REQUIRE(scaled_twice.at(0, 0) == T {0});
    REQUIRE(scaled_twice.at(0, 1) == T {4});
    REQUIRE(scaled_twice.at(1, 0) == T {8});
    REQUIRE(scaled_twice.at(1, 1) == T {12});

    auto add_scaler = T {2} + mat;
    REQUIRE(add_scaler.at(0, 0) == T {2});
    REQUIRE(add_scaler.at(0, 1) == T {3});
    REQUIRE(add_scaler.at(1, 0) == T {4});
    REQUIRE(add_scaler.at(1, 1) == T {5});

    auto sub_scaler = T {2} - mat;
    REQUIRE(sub_scaler.at(0, 0) == T {-2});
    REQUIRE(sub_scaler.at(0, 1) == T {-1});
    REQUIRE(sub_scaler.at(1, 0) == T {0});
    REQUIRE(sub_scaler.at(1, 1) == T {1});

    REQUIRE(add_scaler != sub_scaler);

    auto summed_matrix = add_scaler + sub_scaler;
    REQUIRE(summed_matrix.at(0, 0) == T {0});
    REQUIRE(summed_matrix.at(0, 1) == T {2});
    REQUIRE(summed_matrix.at(1, 0) == T {4});
    REQUIRE(summed_matrix.at(1, 1) == T {6});

    auto sub_matrix = add_scaler - sub_scaler;
    REQUIRE(sub_matrix.at(0, 0) == T {4});
    REQUIRE(sub_matrix.at(0, 1) == T {4});
    REQUIRE(sub_matrix.at(1, 0) == T {4});
    REQUIRE(sub_matrix.at(1, 1) == T {4});

    REQUIRE(sub_matrix != ta::DynamicMatrix<T> {});

    mat.clear();
    REQUIRE(mat.at(0, 0) == T {0});
    REQUIRE(mat.at(0, 1) == T {0});
    REQUIRE(mat.at(1, 0) == T {0});
    REQUIRE(mat.at(1, 1) == T {0});
}

auto main() -> int
{
    vector_test<std::int16_t>();
    vector_test<std::int32_t>();
    vector_test<std::int64_t>();

    vector_test<int>();
    vector_test<float>();
    vector_test<double>();

    matrix_test<std::int16_t>();
    matrix_test<std::int32_t>();
    matrix_test<std::int64_t>();

    matrix_test<int>();
    matrix_test<float>();
    matrix_test<double>();

    return EXIT_SUCCESS;
}