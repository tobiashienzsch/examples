#include <etl/linalg.hpp>

#undef NDEBUG
#include <cassert>
#include <cstdlib>
#include <vector>

auto main() -> int
{
    auto data = std::vector<float>{0.0F, 1.0F, 2.0, 3.0F};
    auto vec  = etl::mdspan<float, etl::dextents<int, 1>>{data.data(), 4};
    assert(vec[0] == 0.0F);
    assert(vec[1] == 1.0F);
    assert(vec[2] == 2.0F);
    assert(vec[3] == 3.0F);

    etl::linalg::scale(2.0F, vec);
    assert(vec[0] == 0.0F);
    assert(vec[1] == 2.0F);
    assert(vec[2] == 4.0F);
    assert(vec[3] == 6.0F);

    return EXIT_SUCCESS;
}