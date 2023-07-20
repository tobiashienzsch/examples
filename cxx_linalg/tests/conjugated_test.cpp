#include <etl/linalg.hpp>

#undef NDEBUG
#include <cassert>
#include <cstdlib>
#include <vector>

auto main() -> int
{
    {
        // real
        auto vec_data = etl::experimental::mdarray<float, etl::dextents<int, 1>>{4};
        auto vec      = vec_data.to_mdspan();

        vec[0] = 0.0F;
        vec[1] = 1.0F;
        vec[2] = 2.0F;
        vec[3] = 3.0F;

        auto const conjugated = etl::linalg::conjugated(vec);
        assert(conjugated[0] == 0.0F);
        assert(conjugated[1] == 1.0F);
        assert(conjugated[2] == 2.0F);
        assert(conjugated[3] == 3.0F);
    }

    {
        // complex
        auto vec_data
            = etl::experimental::mdarray<std::complex<float>, etl::dextents<int, 1>>{4};
        auto vec = vec_data.to_mdspan();

        vec[0] = std::complex{1.0F, 0.0F};
        vec[1] = std::complex{2.0F, 1.0F};
        vec[2] = std::complex{3.0F, 2.0F};
        vec[3] = std::complex{4.0F, 3.0F};

        auto const conjugated = etl::linalg::conjugated(vec);
        static_assert(std::same_as<
                      typename decltype(conjugated)::accessor_type::element_type,
                      std::complex<float> const>);
        static_assert(std::same_as<
                      typename decltype(conjugated)::accessor_type::reference::value_type,
                      std::complex<float>>);

        assert(std::complex<float>{conjugated[0]} == std::conj(std::complex{1.0F, 0.0F}));
        assert(std::complex<float>{conjugated[1]} == std::conj(std::complex{2.0F, 1.0F}));
        assert(std::complex<float>{conjugated[2]} == std::conj(std::complex{3.0F, 2.0F}));
        assert(std::complex<float>{conjugated[3]} == std::conj(std::complex{4.0F, 3.0F}));
    }

    return EXIT_SUCCESS;
}