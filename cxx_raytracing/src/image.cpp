#include "image.hpp"

#include <iterator>
#include <stdexcept>

namespace mc {

Image::Image(int w, int h) : _width{w}, _height{h}, _pixels(static_cast<size_t>(w * h)) {}

auto Image::width() const noexcept -> int { return _width; }

auto Image::height() const noexcept -> int { return _height; }

auto Image::operator()(int x, int y) -> PixelRGB&
{
    return _pixels[static_cast<size_t>(y * _width + x)];
}

auto Image::operator()(int x, int y) const -> PixelRGB const&
{
    return _pixels[static_cast<size_t>(y * _width + x)];
}

auto Image::data() noexcept -> PixelRGB* { return _pixels.data(); }

auto Image::data() const noexcept -> PixelRGB const* { return _pixels.data(); }

auto size(Image const& img) noexcept -> int { return img.width() * img.height(); }

auto begin(Image& img) noexcept -> PixelRGB* { return std::data(img); }

auto end(Image& img) noexcept -> PixelRGB* { return std::data(img) + size(img); }

auto begin(Image const& img) noexcept -> PixelRGB const* { return std::data(img); }

auto end(Image const& img) noexcept -> PixelRGB const*
{
    return std::data(img) + size(img);
}

auto writeToFile(Image const& img, char const* path) -> void
{
    auto file = std::fopen(path, "wb");
    if (file == nullptr) { throw std::runtime_error{"could not open file"}; }

    std::fprintf(file, "P3\n");
    std::fprintf(file, "%d %d\n", img.width(), img.height());
    std::fprintf(file, "255\n");

    for (auto y{img.height() - 1}; y >= 0; --y) {
        for (auto x{0}; x < img.width(); ++x) {
            auto const r = img(x, y).red;
            auto const g = img(x, y).green;
            auto const b = img(x, y).blue;
            std::fprintf(file, "%d %d %d\n", r, g, b);
        }
    }

    std::fclose(file);
}

auto makeImageWithGradient() -> Image
{
    auto img          = Image{200, 200};
    auto const width  = img.width();
    auto const height = img.height();

    for (auto y{0}; y < height; ++y) {
        for (auto x{0}; x < width; ++x) {
            auto const r = static_cast<float>(x) / static_cast<float>(width);
            auto const g = static_cast<float>(y) / static_cast<float>(height);
            auto const b = 0.2F;
            img(x, y)    = PixelRGB::fromFloat(r, g, b);
        }
    }

    return img;
}

auto makeImageWithSolidFill(PixelRGB color) -> Image
{
    auto img = Image{200, 200};
    std::fill(begin(img), end(img), color);
    return img;
}

}  // namespace mc