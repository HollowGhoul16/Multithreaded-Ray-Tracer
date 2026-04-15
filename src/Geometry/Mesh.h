#pragma once

#include <vector>
#include <limits>
#include <algorithm>
#include <memory>

#include "Acceleration/AABB.h"
#include "Transform.h"
#include "Surfaces.h"

struct Mesh {
    AABB aabb;
    std::shared_ptr<std::vector<Triangle>> triangles;
    std::vector<Surface*> surfaces;
    Transform transform;
    Material material;
    bool selected = false;
    static bool wireframeAABB;

    Mesh() = default;

    Mesh(const std::shared_ptr<std::vector<Triangle>>& triangles, const Mat4& modelMatrix = Mat4::identity());

    Mesh(std::vector<Surface*>&& surfaces, const Mat4& modelMatrix = Mat4::identity());

    Mesh(Mesh&& otherMesh);

    ~Mesh();

    const HitData AABBintersection(const Ray& worldRay, const float& tClosestSurface) const;

    const bool AABBcontains(const Vec3& worldPoint) const;

    const HitData intersection(const Ray& worldRay) const;

    Mesh duplicate() const;

    void applyTransform(const Mat4& transformation);

    void toggleSelected();

    void markSelected();

    void markDeselected();
};

#include "Mesh.inl"