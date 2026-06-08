#pragma once

#include "Math/Math.h"

struct RayCone {
    float width = 0.0f;
    float spreadAngle = std::atan((2.0 * std::tan((77.32 * Math::PRECISE_PI / 180.0) / 2.0)) / (400.0)); // Hardcoded (might be wrong)

    void updateWidth(const float& t);

    void updateSpreadAngle(const float& surfaceSpreadAngle);
};

#include "RayCone.inl"