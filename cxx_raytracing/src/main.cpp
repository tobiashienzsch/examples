#include "image.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

using namespace mc;

[[nodiscard]] static auto hit(Sphere<float> const& s, Ray<float> const& r) -> bool
{
    auto const oc = r.origin - s.center;
    auto const a  = dot(r.direction, r.direction);
    auto const b  = dot(oc, r.direction);
    auto const c  = dot(oc, oc) - s.radius * s.radius;

    auto const discriminant = b * b - 4.0F * a * c;
    return discriminant > 0.0F;
}

static auto color(Ray<float> const& ray) -> PixelRGB
{
    auto const sphere = Sphere<float>{
        Vec3<float>{0.0F, 0.0F, -1.0F},
        0.5F
    };
    if (hit(sphere, ray)) { return PixelRGB{255, 0, 0}; }

    auto const unit = makeUnit(ray.direction);
    auto const t    = (unit.y + 1.0F) * 0.5F;
    auto const vec  = Vec3<float>{1, 1, 1} * (1.0F - t) + Vec3<float>{0.5F, 0.7F, 1.0F} * t;

    return PixelRGB::fromFloat(vec.x, vec.y, vec.z);
}

[[nodiscard]] static auto makeImageWithSphere() -> Image
{
    auto img = Image{600, 300};

    auto const lowerLeft  = Vec3<float>{-2.0F, -1.0F, -1.0F};
    auto const horizontal = Vec3<float>{4.0F, 0.0F, 0.0F};
    auto const vertical   = Vec3<float>{0.0F, 2.0F, 0.0F};
    auto const origin     = Vec3<float>{0.0F, 0.0F, 0.0F};

    for (auto y{img.height() - 1}; y >= 0; --y) {
        for (auto x{0}; x < img.width(); ++x) {
            auto const u   = float(x) / float(img.width());
            auto const v   = float(y) / float(img.height());
            auto const dir = lowerLeft + horizontal * u + vertical * v;

            img(x, y) = color(Ray<float>{origin, dir});
        }
    }

    return img;
}

auto main(int, char** argv) -> int
{

    writeToFile(makeImageWithGradient(), argv[1]);

    auto const color = PixelRGB::fromFloat(0.1F, 0.1F, 0.1F);
    auto const img   = makeImageWithSolidFill(color);
    writeToFile(img, argv[1]);

    writeToFile(makeImageWithSphere(), argv[1]);

    return EXIT_SUCCESS;
}