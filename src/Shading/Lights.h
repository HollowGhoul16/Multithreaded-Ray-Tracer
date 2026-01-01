#pragma once

#include "Math/Math.h"

struct DirectionalLight {
    Vec3 direction;
    float intensity;

    DirectionalLight(const Vec3& d, const float& i);
};