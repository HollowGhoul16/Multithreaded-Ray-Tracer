#pragma once

#include <vector>
#include <limits>

#include "Acceleration/AABB.h"
#include "Surfaces.h"

struct Mesh {
    AABB aabb;
    std::vector<Surface*> surfaces;

    Mesh(std::vector<Surface*>&& surfaces);

    Mesh(Mesh&& otherMesh);

    ~Mesh();

    const std::pair<Surface*, float> intersection(const Ray& ray) const;
};

#include "Mesh.inl"