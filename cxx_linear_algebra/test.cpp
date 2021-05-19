#include "linear_solve.hpp"
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
    auto vec = math::Vector<T> {2};
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

    auto other_size = math::Vector<T> {};
    REQUIRE(other_size.size() == 0);
    REQUIRE(vec != other_size);
    REQUIRE(!(vec == other_size));

    auto same_size = math::Vector<T> {3};
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
        auto fail = vec + math::Vector<T> {5};
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto fail = vec - math::Vector<T> {5};
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto lhs = math::Vector<T> {2};
    auto rhs = math::Vector<T> {2};
    lhs[0]   = T {1};
    lhs[1]   = T {2};
    rhs[0]   = T {1};
    rhs[1]   = T {2};
    REQUIRE(math::dotProduct(lhs, rhs) == T {5});

    rhs[0] = T {4};
    rhs[1] = T {4};
    REQUIRE(math::dotProduct(lhs, rhs) == T {12});

    lhs.resize(3);
    rhs.resize(3);
    lhs[0] = T {1};
    lhs[1] = T {2};
    lhs[2] = T {3};

    rhs[0] = T {1};
    rhs[1] = T {2};
    rhs[2] = T {3};

    auto cross = math::crossProduct(lhs, rhs);
    REQUIRE(cross[0] == T {0});
    REQUIRE(cross[1] == T {0});
    REQUIRE(cross[2] == T {0});

    try
    {
        math::crossProduct(lhs, math::Vector<T> {16});
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        constexpr auto const* msg = "vectors need to be the same size";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        math::crossProduct(math::Vector<T> {16}, math::Vector<T> {16});
        REQUIRE(false);
    }
    catch (std::domain_error const& e)
    {
        constexpr auto const* msg = "only 3-dimensional vector supported";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        math::dotProduct(lhs, math::Vector<T> {16});
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

    auto normalized = math::normalized(lhs);
    REQUIRE(normalized[0] == T {2});
    REQUIRE(normalized[1] == T {0});
    REQUIRE(normalized[2] == T {0});

    math::normalize(lhs);
    REQUIRE(lhs[0] == T {2});
    REQUIRE(lhs[1] == T {0});
    REQUIRE(lhs[2] == T {0});
}

template<typename T>
auto matrix_class_test() -> void
{
    auto mat = math::Matrix<T> {};
    REQUIRE(mat.rows() == 0);
    REQUIRE(mat.cols() == 0);
    REQUIRE(mat.size() == 0);
    REQUIRE(!math::isNonZero(mat));

    mat.resize(2, 2);
    REQUIRE(mat.rows() == 2);
    REQUIRE(mat.cols() == 2);
    REQUIRE(mat.size() == 4);

    REQUIRE(!math::isNonZero(mat));
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

    REQUIRE(math::isNonZero(mat));

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

    REQUIRE(sub_matrix != math::Matrix<T> {});

    mat.clear();
    REQUIRE(mat(0, 0) == T {0});
    REQUIRE(mat(0, 1) == T {0});
    REQUIRE(mat(1, 0) == T {0});
    REQUIRE(mat(1, 1) == T {0});
}

template<typename T>
auto matrix_functions_test() -> void
{
    try
    {
        auto swap  = math::Matrix<T> {2, 2};
        swap(0, 0) = T {0};
        swap(0, 1) = T {0};
        swap(1, 0) = T {1};
        swap(1, 1) = T {1};

        math::swapRow(swap, 0, 1);
        REQUIRE(swap(0, 0) == T {1});
        REQUIRE(swap(0, 1) == T {1});
        REQUIRE(swap(1, 0) == T {0});
        REQUIRE(swap(1, 1) == T {0});

        math::swapRow(swap, 0, 2);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto multi  = math::Matrix<T> {2, 2};
        multi(0, 0) = T {0};
        multi(0, 1) = T {0};
        multi(1, 0) = T {1};
        multi(1, 1) = T {1};

        math::multiplyRow(multi, 0, T {2});
        math::multiplyRow(multi, 1, T {2});
        REQUIRE(multi(0, 0) == T {0});
        REQUIRE(multi(0, 1) == T {0});
        REQUIRE(multi(1, 0) == T {2});
        REQUIRE(multi(1, 1) == T {2});

        math::multiplyRow(multi, 2, T {2});
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        auto multiAdd  = math::Matrix<T> {2, 2};
        multiAdd(0, 0) = T {0};
        multiAdd(0, 1) = T {0};
        multiAdd(1, 0) = T {1};
        multiAdd(1, 1) = T {1};

        math::multiplyAddRow(multiAdd, 1, 0, T {2});
        REQUIRE(multiAdd(0, 0) == T {2});
        REQUIRE(multiAdd(0, 1) == T {2});
        REQUIRE(multiAdd(1, 0) == T {1});
        REQUIRE(multiAdd(1, 1) == T {1});

        math::multiplyAddRow(multiAdd, 2, 0, T {2});
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto maxRow  = math::Matrix<T> {2, 2};
    maxRow(0, 0) = T {1};
    maxRow(0, 1) = T {1};
    maxRow(1, 0) = T {0};
    maxRow(1, 1) = T {0};

    REQUIRE(math::findRowWithMaxElement(maxRow, 0, 0) == 0);
    REQUIRE(math::findRowWithMaxElement(maxRow, 0, 1) == 1);

    try
    {
        math::findRowWithMaxElement(maxRow, 0, 2);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "row index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    try
    {
        math::findRowWithMaxElement(maxRow, 2, 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "column index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto split  = math::Matrix<T> {2, 2};
    split(0, 0) = T {1};
    split(0, 1) = T {1};
    split(1, 0) = T {0};
    split(1, 1) = T {0};

    auto splits = math::splitColumns(split, 1);
    REQUIRE(splits.first.rows() == 2);
    REQUIRE(splits.first.cols() == 1);

    REQUIRE(splits.second.rows() == 2);
    REQUIRE(splits.second.cols() == 1);

    try
    {
        REQUIRE(math::splitColumns(split, 2).first.size() == 0);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "column index out of bounds";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto joinA  = math::Matrix<T> {2, 2};
    joinA(0, 0) = T {1};
    joinA(1, 0) = T {1};
    joinA(0, 1) = T {0};
    joinA(1, 1) = T {0};
    auto joinB  = math::Matrix<T> {2, 1};
    joinB(0, 0) = T {1};
    joinB(1, 0) = T {1};

    auto joinedA = math::join(joinA, joinB);
    REQUIRE(joinedA.rows() == 2);
    REQUIRE(joinedA.cols() == 3);
    REQUIRE(joinedA(0, 0) == T {1});
    REQUIRE(joinedA(1, 0) == T {1});
    REQUIRE(joinedA(0, 1) == T {0});
    REQUIRE(joinedA(1, 1) == T {0});
    REQUIRE(joinedA(0, 2) == T {1});
    REQUIRE(joinedA(1, 2) == T {1});

    auto joinC  = math::Matrix<T> {2, 2};
    joinA(0, 0) = T {1};
    joinA(1, 0) = T {1};
    joinA(0, 1) = T {0};
    joinA(1, 1) = T {0};
    auto joinD  = math::Matrix<T> {2, 2};
    joinB(0, 0) = T {1};
    joinB(1, 0) = T {1};

    auto joinedB = math::join(joinC, joinD);
    REQUIRE(joinedB.rows() == 2);
    REQUIRE(joinedB.cols() == 4);

    try
    {
        REQUIRE((math::join(joinA, math::Matrix<T> {3, 1}).size() == 0));
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "rows size must match";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto identityA = math::Matrix<T> {2, 2};
    math::makeIdentity(identityA);
    identityA(0, 0) = T {1};
    identityA(1, 0) = T {0};
    identityA(0, 1) = T {0};
    identityA(1, 1) = T {1};

    auto identityB  = math::makeIdentity<T>(3);
    identityB(0, 0) = T {1};
    identityB(1, 0) = T {0};
    identityB(0, 1) = T {0};
    identityB(1, 1) = T {1};
    identityB(2, 2) = T {1};

    try
    {
        auto identityC = math::Matrix<T> {2, 1};
        math::makeIdentity(identityC);
        REQUIRE(false);
    }
    catch (std::exception const& e)
    {
        auto const* msg = "matrix must be square";
        REQUIRE((std::strcmp(e.what(), msg) == 0));
    }

    auto invertA  = math::Matrix<T> {3, 3};
    invertA(0, 0) = T {2};
    invertA(0, 1) = T {1};
    invertA(0, 2) = T {1};
    invertA(1, 0) = T {1};
    invertA(1, 1) = T {0};
    invertA(1, 2) = T {1};
    invertA(2, 0) = T {0};
    invertA(2, 1) = T {3};
    invertA(2, 2) = T {1};

    auto const invA = math::inverse(invertA);
    REQUIRE(invA(0, 0) == T {0.75});
    REQUIRE(invA(0, 1) == T {-0.5});
    REQUIRE(invA(0, 2) == T {-0.25});
    REQUIRE(invA(1, 0) == T {0.25});
    REQUIRE(invA(1, 1) == T {-0.5});
    REQUIRE(invA(1, 2) == T {0.25});
    REQUIRE(invA(2, 0) == T {-0.75});
    REQUIRE(invA(2, 1) == T {1.5});
    REQUIRE(invA(2, 2) == T {0.25});

    auto sub = math::subMatrix(invertA, 0, 0);
    REQUIRE(math::isSquare(sub));
    REQUIRE(sub.rows() == 2);
    REQUIRE(sub(0, 0) == T {0});
    REQUIRE(sub(0, 1) == T {1});
    REQUIRE(sub(1, 0) == T {3});
    REQUIRE(sub(1, 1) == T {1});

    REQUIRE(math::determinant(invertA) == T {-4});

    REQUIRE(math::isRowEchelon(math::makeIdentity<T>(2)));
    REQUIRE(math::isRowEchelon(math::makeIdentity<T>(3)));
    REQUIRE(math::isRowEchelon(math::makeIdentity<T>(4)));
    REQUIRE(math::isRowEchelon(math::makeIdentity<T>(5)));
    REQUIRE(math::isRowEchelon(math::makeIdentity<T>(6)));
    REQUIRE(math::isRowEchelon(math::makeIdentity<T>(7)));

    auto id2 = math::makeIdentity<T>(2);
    math::rowEchelon(id2);
    REQUIRE(math::isRowEchelon(id2));

    auto id3 = math::makeIdentity<T>(3);
    math::rowEchelon(id3);
    REQUIRE(math::isRowEchelon(id3));

    auto id4 = math::makeIdentity<T>(4);
    math::rowEchelon(id4);
    REQUIRE(math::isRowEchelon(id4));

    auto id5 = math::makeIdentity<T>(5);
    math::rowEchelon(id5);
    REQUIRE(math::isRowEchelon(id5));

    auto id6 = math::makeIdentity<T>(6);
    math::rowEchelon(id6);
    REQUIRE(math::isRowEchelon(id6));

    auto id7 = math::makeIdentity<T>(7);
    math::rowEchelon(id7);
    REQUIRE(math::isRowEchelon(id7));

    auto A  = math::Matrix<T> {3, 4};
    A(0, 0) = T {1};
    A(0, 1) = T {3};
    A(0, 2) = T {-1};
    A(0, 3) = T {13};
    A(1, 0) = T {4};
    A(1, 1) = T {-1};
    A(1, 2) = T {1};
    A(1, 3) = T {9};
    A(2, 0) = T {2};
    A(2, 1) = T {4};
    A(2, 2) = T {3};
    A(2, 3) = T {-6};
    math::rowEchelon(A);
    REQUIRE(math::isRowEchelon(A));
    REQUIRE(math::rank(A) == 3);
    REQUIRE(math::isNonZero(A));

    A.resize(3, 3);
    A(0, 0) = T {0};
    A(0, 1) = T {0};
    A(0, 2) = T {1};
    A(1, 0) = T {1};
    A(1, 1) = T {0};
    A(1, 2) = T {1};
    A(2, 0) = T {0};
    A(2, 1) = T {0};
    A(2, 2) = T {1};
    REQUIRE(math::rank(A) == 2);
    REQUIRE(math::isNonZero(A));
}

template<typename T>
auto linear_solve_test() -> void
{
    auto A  = math::Matrix<T> {3, 3};
    A(0, 0) = T {1};
    A(0, 1) = T {3};
    A(0, 2) = T {-1};
    A(1, 0) = T {4};
    A(1, 1) = T {-1};
    A(1, 2) = T {1};
    A(2, 0) = T {2};
    A(2, 1) = T {4};
    A(2, 2) = T {3};

    auto b = math::Vector<T> {3};
    b[0]   = T {13};
    b[1]   = T {9};
    b[2]   = T {-6};

    auto result = math::linearSolve(A, b);
    REQUIRE(result.vec.size() == 3);
    REQUIRE(result.success == math::LinearSolveSolution::Unique);
    REQUIRE(math::closeEnough(result.vec[0], T {4}));
    REQUIRE(math::closeEnough(result.vec[1], T {1}));
    REQUIRE(math::closeEnough(result.vec[2], T {-6}));

    A.clear();
    b.clear();
    A(0, 0) = T {1};
    A(0, 1) = T {0};
    A(0, 2) = T {0};
    A(1, 0) = T {0};
    A(1, 1) = T {1};
    A(1, 2) = T {0};
    A(2, 0) = T {1};
    A(2, 1) = T {1};
    A(2, 2) = T {0};
    result  = math::linearSolve(A, b);
    REQUIRE(result.success == math::LinearSolveSolution::NoUnique);

    A.clear();
    b.clear();
    A(0, 0) = T {1};
    A(0, 1) = T {0};
    A(0, 2) = T {0};
    A(1, 0) = T {1};
    A(1, 1) = T {0};
    A(1, 2) = T {0};
    A(2, 0) = T {1};
    A(2, 1) = T {0};
    A(2, 2) = T {0};
    b[0]    = T {0};
    b[1]    = T {-1};
    b[2]    = T {1};
    result  = math::linearSolve(A, b);
    REQUIRE(result.success == math::LinearSolveSolution::None);
}

auto main() -> int
{
    vector_test<float>();
    vector_test<double>();
    vector_test<long double>();

    matrix_class_test<float>();
    matrix_class_test<double>();
    matrix_class_test<long double>();

    matrix_functions_test<float>();
    matrix_functions_test<double>();
    matrix_functions_test<long double>();

    linear_solve_test<float>();
    linear_solve_test<double>();
    linear_solve_test<long double>();

    return EXIT_SUCCESS;
}