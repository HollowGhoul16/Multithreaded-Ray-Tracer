#pragma once

#include "Math/Math.h"
#include "Geometry/HitData.hpp"
#include "Geometry/Surfaces.h"

struct AABB {
    Vec3 min, max;
    Rectangle wireframe[24];

    HitData intersection(const Ray& ray, const float& tClosestMax) const;

    void constructWireframe();
};

#include "AABB.inl"