#pragma once

#include <algorithm>
#include <cstdint>

struct PixelRGB
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    static auto fromFloat(float r, float g, float b) noexcept -> PixelRGB;
};

inline auto PixelRGB::fromFloat(float r, float g, float b) noexcept -> PixelRGB
{
    return {
        static_cast<uint8_t>(std::clamp(r * 256.0F, 0.0F, 256.0F)),
        static_cast<uint8_t>(std::clamp(g * 256.0F, 0.0F, 256.0F)),
        static_cast<uint8_t>(std::clamp(b * 256.0F, 0.0F, 256.0F)),
    };
}