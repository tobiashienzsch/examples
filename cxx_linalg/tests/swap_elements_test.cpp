#include <etl/linalg.hpp>

#undef NDEBUG
#include <cassert>
#include <cstdlib>
#include <vector>

auto main() -> int
{
    auto data      = etl::experimental::mdarray<float, etl::dextents<int, 1>>{4};
    auto otherData = etl::experimental::mdarray<float, etl::dextents<int, 1>>{4};

    auto vec      = data.to_mdspan();
    auto otherVec = otherData.to_mdspan();

    vec[0] = 0.0F;
    vec[1] = 1.0F;
    vec[2] = 2.0F;
    vec[3] = 3.0F;

    etl::linalg::swap_elements(vec, otherVec);
    assert(vec[0] == 0.0F);
    assert(vec[1] == 0.0F);
    assert(vec[2] == 0.0F);
    assert(vec[3] == 0.0F);

    assert(otherVec[0] == 0.0F);
    assert(otherVec[1] == 1.0F);
    assert(otherVec[2] == 2.0F);
    assert(otherVec[3] == 3.0F);

    return EXIT_SUCCESS;
}