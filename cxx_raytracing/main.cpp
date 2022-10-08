#include "image.hpp"

auto makeGradient() -> Image
{
    auto img          = Image {200, 200};
    auto const width  = img.width();
    auto const height = img.height();

    for (auto y {0}; y < height; ++y)
    {
        for (auto x {0}; x < width; ++x)
        {
            auto const r = static_cast<float>(x) / static_cast<float>(width);
            auto const g = static_cast<float>(y) / static_cast<float>(height);
            auto const b = 0.2F;
            img(x, y)    = PixelRGB::fromFloat(r, g, b);
        }
    }

    return img;
}

auto makeSolidFill(PixelRGB color) -> Image
{
    auto img = Image {200, 200};
    std::fill(begin(img), end(img), color);
    return img;
}

auto main(int, char** argv) -> int
{
    writeToFile(makeGradient(), argv[1]);

    // auto const color = PixelRGB::fromFloat(0.1F, 0.1F, 0.1F);
    // auto const img   = makeSolidFill(color);
    // writeToFile(img, argv[1]);

    return EXIT_SUCCESS;
}