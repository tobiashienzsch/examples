#pragma once

#include <vector>

#include "pixel_rgb.hpp"

namespace mc {

struct Image
{
    Image(int w, int h);

    [[nodiscard]] auto width() const noexcept -> int;
    [[nodiscard]] auto height() const noexcept -> int;

    [[nodiscard]] auto operator()(int x, int y) -> PixelRGB&;
    [[nodiscard]] auto operator()(int x, int y) const -> PixelRGB const&;

    [[nodiscard]] auto data() noexcept -> PixelRGB*;
    [[nodiscard]] auto data() const noexcept -> PixelRGB const*;

private:
    int _width;
    int _height;
    std::vector<PixelRGB> _pixels;
};

[[nodiscard]] auto size(Image const& img) noexcept -> int;

[[nodiscard]] auto begin(Image& img) noexcept -> PixelRGB*;
[[nodiscard]] auto end(Image& img) noexcept -> PixelRGB*;

[[nodiscard]] auto begin(Image const& img) noexcept -> PixelRGB const*;
[[nodiscard]] auto end(Image const& img) noexcept -> PixelRGB const*;

auto writeToFile(Image const& img, char const* path) -> void;

[[nodiscard]] auto makeImageWithGradient() -> Image;
[[nodiscard]] auto makeImageWithSolidFill(PixelRGB color) -> Image;

}  // namespace mc