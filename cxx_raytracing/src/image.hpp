#pragma once

#include "pixel_rgb.hpp"

#include <vector>

struct Image
{
    Image() = default;
    Image(int w, int h);

    [[nodiscard]] auto size() const noexcept -> int;
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

[[nodiscard]] auto begin(Image& i) -> PixelRGB*;
[[nodiscard]] auto end(Image& i) -> PixelRGB*;

[[nodiscard]] auto begin(Image const& i) -> PixelRGB const*;
[[nodiscard]] auto end(Image const& i) -> PixelRGB const*;

auto writeToFile(Image const& img, char const* path) -> void;

[[nodiscard]] auto makeGradient() -> Image;
[[nodiscard]] auto makeSolidFill(PixelRGB color) -> Image;