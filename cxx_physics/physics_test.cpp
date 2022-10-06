#include "physics/Vec3.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <span>
#include <vector>

template<typename T>
using Vector = std::vector<T>;

struct Object
{
    Vec3<float> position;
    Vec3<float> velocity;
    Vec3<float> force;
    float mass;
};

struct CollisionPoints
{
    Vec3<float> a;
    Vec3<float> b;
    Vec3<float> normal;
    float depth;
    bool hasCollision;
};

struct Transform
{
    Vec3<float> translation;
    Vec3<float> scale;
    Vec3<float> rotation;
};

struct ShpereCollider
{
    Vec3<float> center;
    float radius;
};

struct PlaneCollider
{
    Vec3<float> plane;
    float distance;
};

struct Collision
{
    Collision(Object* a, Object* b, CollisionPoints const& points) noexcept
        : a {a}, b {b}, points {points}
    {
    }

    Object* a;
    Object* b;
    CollisionPoints points;
};

[[nodiscard]] inline auto
shpereVsShpereCollisionPoints(ShpereCollider const& a, Transform const& ta,
                              ShpereCollider const& b,
                              Transform const& tb) noexcept -> CollisionPoints
{
    (void)ta;
    (void)tb;

    auto const ac       = a.center;
    auto const bc       = b.center;
    auto const distance = std::sqrt((ac.x - bc.x) * (ac.x - bc.x)
                                    + (ac.y - bc.y) * (ac.y - bc.y)
                                    + (ac.z - bc.z) * (ac.z - bc.z));

    return {
        .a            = {},
        .b            = {},
        .normal       = {},
        .depth        = {},
        .hasCollision = distance < (a.radius + b.radius),
    };
}

[[nodiscard]] auto shpereVsPlaneCollisionPoints(ShpereCollider const& a,
                                                Transform const& ta,
                                                PlaneCollider const& b,
                                                Transform const& tb) noexcept
    -> CollisionPoints
{
    (void)a;
    (void)ta;
    (void)b;
    (void)tb;
    return {};
}

struct PhysicsWorld
{
    PhysicsWorld() = default;

    auto objects() noexcept -> std::span<Object*> { return _objects; }
    auto shpereColliders() noexcept -> std::span<ShpereCollider*>
    {
        return _sphereColliders;
    }
    auto planeColliders() noexcept -> std::span<PlaneCollider*>
    {
        return _planeColliders;
    }

    auto addObject(Object* obj) -> void
    {
        assert(obj != nullptr);
        _objects.push_back(obj);
    }

    auto removeObject(Object* obj) -> void
    {
        if (obj == nullptr) return;
        auto it = std::find(std::begin(_objects), std::end(_objects), obj);
        if (it == std::end(_objects)) return;
        _objects.erase(it);
    }

    Vec3<float> gravity {0, -9.81f, 0};

private:
    Vector<Object*> _objects;
    Vector<ShpereCollider*> _sphereColliders;
    Vector<PlaneCollider*> _planeColliders;
};

auto step(PhysicsWorld& world, float dt) -> void
{
    for (auto* obj : world.objects())
    {
        obj->force += obj->mass * world.gravity;

        obj->velocity += obj->force / obj->mass * dt;
        obj->position += obj->velocity * dt;

        obj->force = Vec3<float> {0, 0, 0};
    }
}

auto resolveCollisions(PhysicsWorld& world, float /*dt*/,
                       std::span<Collision> collisions) -> void
{
    auto* out = collisions.data();
    for (auto const* a : world.shpereColliders())
    {
        for (auto const* b : world.shpereColliders())
        {
            if (a == b) break;
            auto const points = shpereVsShpereCollisionPoints(*a, {}, *b, {});
            if (points.hasCollision)
                *(out++) = Collision {nullptr, nullptr, points};
        }
        for (auto const* b : world.planeColliders())
        {
            auto const points = shpereVsPlaneCollisionPoints(*a, {}, *b, {});
            if (points.hasCollision)
                *(out++) = Collision {nullptr, nullptr, points};
        }
    }
}

constexpr auto test_Vec3() -> bool
{
    // empty
    assert(Vec3<int> {} == Vec3<int> {});
    assert(Vec3<float> {} == Vec3<float> {});
    assert(Vec3<double> {} == Vec3<double> {});

    // fill
    auto const v1 = Vec3<int> {1};
    assert(v1.x == 1);
    assert(v1.y == 1);
    assert(v1.z == 1);

    // init all
    auto const v2 = Vec3<int> {1, 2, 3};
    assert(v2.x == 1);
    assert(v2.y == 2);
    assert(v2.z == 3);

    // equal
    assert(v1 == v1);
    assert(v1 != v2);
    assert(v1 != Vec3<int> {});

    return true;
}

static_assert(test_Vec3());

auto main() -> int { return EXIT_SUCCESS; }