#pragma once

#include "Shading/SurfaceProperties.h"
#include <utility>

struct Surface {
    Material material;

    constexpr Surface(const Material& m);

    virtual ~Surface() = default;

    virtual std::pair<bool, float> intersection(const Ray& ray) const = 0;

    virtual Vec3 normal(const Ray& ray, const Vec3& point) const = 0;
};

struct Sphere : Surface {
    Vec3 center;
    float radius;

    constexpr Sphere(const Vec3& cent, const float& r, const Material& m);

    std::pair<bool, float> intersection(const Ray& ray) const override;

    Vec3 normal(const Ray& ray, const Vec3& point) const override;
};

struct Plane : Surface {
    Vec3 point, normalVec;

    constexpr Plane(const Vec3& p, const Vec3& n, const Material& m);

    virtual std::pair<bool, float> intersection(const Ray& ray) const override;

    Vec3 normal(const Ray& ray, const Vec3& point) const override;
};

struct Rectangle : Plane {
    Vec3 corner, edge1, edge2;

    constexpr Rectangle(
        const Vec3& c,
        const Vec3& n,
        const Vec3& e1,
        const Vec3& e2,
        const Material& m
    );

    std::pair<bool, float> intersection(const Ray& ray) const override;

    Vec3 normal(const Ray& ray, const Vec3& point) const override;
};

#include "Surfaces.inl"