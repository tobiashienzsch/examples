#include "pixel_rgb.hpp"

#include <algorithm>

namespace mc {

auto PixelRGB::fromFloat(float r, float g, float b) noexcept -> PixelRGB
{
    return {
        static_cast<uint8_t>(std::clamp(r * 255.0F, 0.0F, 255.0F)),
        static_cast<uint8_t>(std::clamp(g * 255.0F, 0.0F, 255.0F)),
        static_cast<uint8_t>(std::clamp(b * 255.0F, 0.0F, 255.0F)),
    };
}

}  // namespace mc
