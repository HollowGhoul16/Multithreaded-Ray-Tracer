#pragma once

#include "Math/Math.h"

struct AABB {
    Vec3 min, max;

    AABB() = default;

    std::pair<bool, float> intersection(const Ray& ray, const float& tClosestMax) const;
};

#include "AABB.inl"