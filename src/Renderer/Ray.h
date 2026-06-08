#pragma once

#include "Math/Vec.h"
#include "RayCone.h"

struct Ray {
    Vec3 origin, direction;
    RayCone cone;

    Ray() = default;

    Ray(const Vec3& o, const Vec3& d);

    Vec3 parametrize(const float& t) const;
};

#include "Ray.inl"