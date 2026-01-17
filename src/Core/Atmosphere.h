#pragma once

#include "Shading/Lights.h"
#include "Math/Math.h"

struct Atmosphere {
    DirectionalLight sun;
    float sunSize;
    Color skyColor, horizonColor;

    Atmosphere(
        const DirectionalLight& sunDir,
        const float& sunSize,
        const Color& skyColor,
        const Color& horColor
    );

    Color skyModel(const Ray& ray) const;
};

#include "Atmosphere.inl"