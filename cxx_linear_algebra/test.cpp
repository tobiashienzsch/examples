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
    auto vec = ta::Vector<T> {2};
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

    auto other_size = ta::Vector<T> {};
    REQUIRE(other_size.size() == 0);
    REQUIRE(vec != other_size);
    REQUIRE(!(vec == other_size));

    auto same_size = ta::Vector<T> {3};
    REQUIRE(vec != same_size);
    REQUIRE(!(vec == same_size));

    vec.clear();
    REQUIRE(vec == same_size);
    REQUIRE(!(vec != same_size));

    auto const zero_add = vec + same_size;
    REQUIRE(zero_add.size() == 3);
    REQUIRE(zero_add[0] == T {});
    REQUIRE(zero_add[1] == T {});
    REQUIRE(zero_add[2] == T {});

    auto const zero_sub = vec - same_size;
    REQUIRE(zero_sub.size() == 3);
    REQUIRE(zero_sub.at(0) == T {});
    REQUIRE(zero_sub.at(1) == T {});
    REQUIRE(zero_sub.at(2) == T {});

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
        auto fail = vec + ta::Vector<T> {5};
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto fail = vec - ta::Vector<T> {5};
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto lhs = ta::Vector<T> {2};
    auto rhs = ta::Vector<T> {2};
    lhs[0]   = T {1};
    lhs[1]   = T {2};
    rhs[0]   = T {1};
    rhs[1]   = T {2};
    REQUIRE(ta::dotProduct(lhs, rhs) == T {5});

    rhs[0] = T {4};
    rhs[1] = T {4};
    REQUIRE(ta::dotProduct(lhs, rhs) == T {12});

    lhs.resize(3);
    rhs.resize(3);
    lhs[0] = T {1};
    lhs[1] = T {2};
    lhs[2] = T {3};

    rhs[0] = T {1};
    rhs[1] = T {2};
    rhs[2] = T {3};

    auto cross = ta::crossProduct(lhs, rhs);
    REQUIRE(cross[0] == T {0});
    REQUIRE(cross[1] == T {0});
    REQUIRE(cross[2] == T {0});

    try
    {
        ta::crossProduct(lhs, ta::Vector<T> {16});
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        constexpr auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        ta::crossProduct(ta::Vector<T> {16}, ta::Vector<T> {16});
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        constexpr auto const* msg = "only 3-dimensional vector supported";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        ta::dotProduct(lhs, ta::Vector<T> {16});
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto stream = std::stringstream {};
    stream << lhs;
    REQUIRE(stream.str() == "1 2 3 ");

    lhs[0] = T {4};
    lhs[1] = T {0};
    lhs[2] = T {0};

    auto normalized = ta::normalized(lhs);
    REQUIRE(normalized[0] == T {2});
    REQUIRE(normalized[1] == T {0});
    REQUIRE(normalized[2] == T {0});

    ta::normalize(lhs);
    REQUIRE(lhs[0] == T {2});
    REQUIRE(lhs[1] == T {0});
    REQUIRE(lhs[2] == T {0});
}

template<typename T>
auto matrix_test() -> void
{
    auto mat = ta::Matrix<T> {};
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

    try
    {
        REQUIRE(mat.at(2, 0) == 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        REQUIRE(mat.at(0, 2) == 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "column index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto const& cmat = mat;
    REQUIRE(cmat.at(0, 0) == T {0});
    REQUIRE(cmat.at(0, 1) == T {0});
    REQUIRE(cmat.at(1, 0) == T {0});
    REQUIRE(cmat.at(1, 1) == T {0});

    try
    {
        REQUIRE(cmat.at(2, 0) == 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        REQUIRE(cmat.at(0, 2) == 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "column index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    mat(0, 0) = T {0};
    mat(0, 1) = T {1};
    mat(1, 0) = T {2};
    mat(1, 1) = T {3};

    auto stream = std::stringstream {};
    stream << mat;
    REQUIRE(stream.str() == "0 1 \n2 3 \n");

    auto const& const_mat = mat;
    REQUIRE(const_mat(0, 0) == T {0});
    REQUIRE(const_mat(0, 1) == T {1});
    REQUIRE(const_mat(1, 0) == T {2});
    REQUIRE(const_mat(1, 1) == T {3});

    auto scaled = mat * T {2};
    REQUIRE(scaled(0, 0) == T {0});
    REQUIRE(scaled(0, 1) == T {2});
    REQUIRE(scaled(1, 0) == T {4});
    REQUIRE(scaled(1, 1) == T {6});

    auto scaled_twice = T {2} * scaled;
    REQUIRE(scaled_twice(0, 0) == T {0});
    REQUIRE(scaled_twice(0, 1) == T {4});
    REQUIRE(scaled_twice(1, 0) == T {8});
    REQUIRE(scaled_twice(1, 1) == T {12});

    auto add_scaler = T {2} + mat;
    REQUIRE(add_scaler(0, 0) == T {2});
    REQUIRE(add_scaler(0, 1) == T {3});
    REQUIRE(add_scaler(1, 0) == T {4});
    REQUIRE(add_scaler(1, 1) == T {5});

    auto sub_scaler = T {2} - mat;
    REQUIRE(sub_scaler(0, 0) == T {-2});
    REQUIRE(sub_scaler(0, 1) == T {-1});
    REQUIRE(sub_scaler(1, 0) == T {0});
    REQUIRE(sub_scaler(1, 1) == T {1});

    REQUIRE(add_scaler != sub_scaler);

    auto summed_matrix = add_scaler + sub_scaler;
    REQUIRE(summed_matrix(0, 0) == T {0});
    REQUIRE(summed_matrix(0, 1) == T {2});
    REQUIRE(summed_matrix(1, 0) == T {4});
    REQUIRE(summed_matrix(1, 1) == T {6});

    auto sub_matrix = add_scaler - sub_scaler;
    REQUIRE(sub_matrix(0, 0) == T {4});
    REQUIRE(sub_matrix(0, 1) == T {4});
    REQUIRE(sub_matrix(1, 0) == T {4});
    REQUIRE(sub_matrix(1, 1) == T {4});

    REQUIRE(sub_matrix != ta::Matrix<T> {});

    mat.clear();
    REQUIRE(mat(0, 0) == T {0});
    REQUIRE(mat(0, 1) == T {0});
    REQUIRE(mat(1, 0) == T {0});
    REQUIRE(mat(1, 1) == T {0});

    try
    {
        auto swap  = ta::Matrix<T> {2, 2};
        swap(0, 0) = T {0};
        swap(0, 1) = T {0};
        swap(1, 0) = T {1};
        swap(1, 1) = T {1};

        ta::swapRow(swap, 0, 1);
        REQUIRE(swap(0, 0) == T {1});
        REQUIRE(swap(0, 1) == T {1});
        REQUIRE(swap(1, 0) == T {0});
        REQUIRE(swap(1, 1) == T {0});

        ta::swapRow(swap, 0, 2);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto multi  = ta::Matrix<T> {2, 2};
        multi(0, 0) = T {0};
        multi(0, 1) = T {0};
        multi(1, 0) = T {1};
        multi(1, 1) = T {1};

        ta::multiplyRow(multi, 0, T {2});
        ta::multiplyRow(multi, 1, T {2});
        REQUIRE(multi(0, 0) == T {0});
        REQUIRE(multi(0, 1) == T {0});
        REQUIRE(multi(1, 0) == T {2});
        REQUIRE(multi(1, 1) == T {2});

        ta::multiplyRow(multi, 2, T {2});
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto multiAdd  = ta::Matrix<T> {2, 2};
        multiAdd(0, 0) = T {0};
        multiAdd(0, 1) = T {0};
        multiAdd(1, 0) = T {1};
        multiAdd(1, 1) = T {1};

        ta::multiplyAddRow(multiAdd, 1, 0, T {2});
        REQUIRE(multiAdd(0, 0) == T {2});
        REQUIRE(multiAdd(0, 1) == T {2});
        REQUIRE(multiAdd(1, 0) == T {1});
        REQUIRE(multiAdd(1, 1) == T {1});

        ta::multiplyAddRow(multiAdd, 2, 0, T {2});
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto maxRow  = ta::Matrix<T> {2, 2};
    maxRow(0, 0) = T {1};
    maxRow(0, 1) = T {1};
    maxRow(1, 0) = T {0};
    maxRow(1, 1) = T {0};

    REQUIRE(ta::findRowWithMaxElement(maxRow, 0, 0) == 0);
    REQUIRE(ta::findRowWithMaxElement(maxRow, 0, 1) == 1);

    try
    {
        ta::findRowWithMaxElement(maxRow, 0, 2);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        ta::findRowWithMaxElement(maxRow, 2, 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "column index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto split  = ta::Matrix<T> {2, 2};
    split(0, 0) = T {1};
    split(0, 1) = T {1};
    split(1, 0) = T {0};
    split(1, 1) = T {0};

    auto splits = ta::splitColumns(split, 1);
    REQUIRE(splits.first.rows() == 2);
    REQUIRE(splits.first.cols() == 1);

    REQUIRE(splits.second.rows() == 2);
    REQUIRE(splits.second.cols() == 1);

    try
    {
        REQUIRE(ta::splitColumns(split, 2).first.size() == 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "column index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto joinA  = ta::Matrix<T> {2, 2};
    joinA(0, 0) = T {1};
    joinA(1, 0) = T {1};
    joinA(0, 1) = T {0};
    joinA(1, 1) = T {0};
    auto joinB  = ta::Matrix<T> {2, 1};
    joinB(0, 0) = T {1};
    joinB(1, 0) = T {1};

    auto joined = ta::join(joinA, joinB);
    REQUIRE(joined.rows() == 2);
    REQUIRE(joined.cols() == 3);
    REQUIRE(joined(0, 0) == T {1});
    REQUIRE(joined(1, 0) == T {1});
    REQUIRE(joined(0, 1) == T {0});
    REQUIRE(joined(1, 1) == T {0});
    REQUIRE(joined(0, 2) == T {1});
    REQUIRE(joined(1, 2) == T {1});

    try
    {
        REQUIRE((ta::join(joinA, ta::Matrix<T> {3, 1}).size() == 0));
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "rows size must match";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }
}

auto main() -> int
{
    vector_test<float>();
    vector_test<double>();

    matrix_test<float>();
    matrix_test<double>();

    return EXIT_SUCCESS;
}