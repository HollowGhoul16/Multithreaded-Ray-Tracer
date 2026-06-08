#pragma once

#include <utility>

#include "HitData.hpp"
#include "Shading/SurfaceProperties.h"

struct Surface {
    Material material;
    SurfaceType type;

    Surface(const Material& m, const SurfaceType& t);

    virtual ~Surface() = default;

    virtual HitData intersection(const Ray& ray) const = 0;

    virtual Vec3 normal(const Ray& ray, const Vec3& point) const = 0;

    virtual std::pair<Vec3, Vec3> getBounds() const = 0;
};

struct Sphere : Surface {
    Vec3 center;
    float radius;

    Sphere();

    Sphere(const Vec3& cent, const float& r, const Material& m);

    HitData intersection(const Ray& ray) const override;

    Vec3 normal(const Ray& ray, const Vec3& point) const override;

    std::pair<Vec3, Vec3> getBounds() const override;
};

struct Plane : Surface {
    Vec3 point, normalVec;

    Plane(const Vec3& p, const Vec3& n, const Material& m);

    HitData intersection(const Ray& ray) const override;

    Vec3 normal(const Ray& ray, const Vec3& point) const override;

    virtual std::pair<Vec3, Vec3> getBounds() const override = 0;
};

struct Rectangle : Plane {
    Vec3 corner, edge1, edge2;

    Rectangle(); // Meant for AABB wireframe construction

    Rectangle(
        const Vec3& c,
        const Vec3& n,
        const Vec3& e1,
        const Vec3& e2,
        const Material& m
    );

    HitData intersection(const Ray& ray) const override;

    Vec3 normal(const Ray& ray, const Vec3& point) const override;

    std::pair<Vec3, Vec3> getBounds() const override;
};

struct Triangle : Plane {
    Vec3 vertices[3];
    Vec3 edges[3];
    Vec2 texCoords[3];
    Vec3 normals[3];

    Triangle(const Vec3 v[3], const Vec2 tc[3], const Vec3 n[3], const Material& m);

    HitData intersection(const Ray& ray) const override; // Möller-Trumbore algorithm

    Vec3 normal(const Ray& ray, const Vec3& point) const override;

    std::pair<Vec3, Vec3> getBounds() const override;

    float getLODConstant(const Mat3& modelMatrix) const;
};

#include "Surfaces.inl"