#pragma once

#include "pixel_rgb.hpp"

#include <stdexcept>
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

auto begin(Image& i) -> PixelRGB* { return i.data(); }
auto end(Image& i) -> PixelRGB* { return i.data() + i.size(); }

auto begin(Image const& i) -> PixelRGB const* { return i.data(); }
auto end(Image const& i) -> PixelRGB const* { return i.data() + i.size(); }

auto writeToFile(Image const& img, char const* path) -> void;

inline Image::Image(int w, int h)
    : _width {w}, _height {h}, _pixels(static_cast<size_t>(w * h))
{
}

inline auto Image::size() const noexcept -> int { return _width * _height; }
inline auto Image::width() const noexcept -> int { return _width; }
inline auto Image::height() const noexcept -> int { return _height; }

inline auto Image::operator()(int x, int y) -> PixelRGB&
{
    return _pixels[static_cast<size_t>(y * _width + x)];
}

inline auto Image::operator()(int x, int y) const -> PixelRGB const&
{
    return _pixels[static_cast<size_t>(y * _width + x)];
}

inline auto Image::data() noexcept -> PixelRGB* { return _pixels.data(); }
inline auto Image::data() const noexcept -> PixelRGB const*
{
    return _pixels.data();
}

inline auto writeToFile(Image const& img, char const* path) -> void
{
    auto file = std::fopen(path, "wb");
    if (file == nullptr)
    {
        throw std::runtime_error {"could not open file"};
    }

    std::fprintf(file, "P3\n");
    std::fprintf(file, "%d %d\n", img.width(), img.height());
    std::fprintf(file, "255\n");

    for (auto y {img.height() - 1}; y >= 0; --y)
    {
        for (auto x {0}; x < img.width(); ++x)
        {
            auto const r = img(x, y).red;
            auto const g = img(x, y).green;
            auto const b = img(x, y).blue;
            std::fprintf(file, "%d %d %d\n", r, g, b);
        }
    }

    std::fclose(file);
}
