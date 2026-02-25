#pragma once

#include "Vec.h"

struct Ray {
    Vec3 origin, direction;

    Ray() = default;

    Ray(const Vec3& o, const Vec3& d);

    Vec3 parametrize(const float& t) const;
};

#include "Ray.inl"