#include "image.hpp"

auto main(int, char** argv) -> int
{
    // writeToFile(makeGradient(), argv[1]);

    auto const color = PixelRGB::fromFloat(0.1F, 0.1F, 0.1F);
    auto const img   = makeSolidFill(color);
    writeToFile(img, argv[1]);

    return EXIT_SUCCESS;
}