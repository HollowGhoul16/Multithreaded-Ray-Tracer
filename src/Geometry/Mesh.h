#pragma once

#include <vector>
#include <limits>
#include <algorithm>

#include "Acceleration/AABB.h"
#include "Surfaces.h"

struct Mesh {
    AABB aabb;
    std::vector<Surface*> surfaces;
    static bool wireframeAABB;

    Mesh() = default;

    Mesh(std::vector<Surface*>&& surfaces);

    Mesh(Mesh&& otherMesh);

    ~Mesh();

    const HitData intersection(const Ray& ray) const;
};

#include "Mesh.inl"