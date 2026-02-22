#pragma once

#include "Color.h"
#include "Math/Math.h"
#include "Lights.h"

// Sum of the coefficients should be between [0, 1]
struct Material {
    Color ambientColor, diffuseColor, specularColor;
    float ambientCoeff, diffuseCoeff, specularCoeff, specularExp;
    bool isGlazed, isMirror, isEmissive;

    Material() = default;

    Material(const Color& color); // For constructing emissive surfaces (in this case, point lights)

    Material(const Color& color, const float& spe_coeff); // For constructing mirrors

    Material(
        const Color& ac,
        const Color& dc,
        const Color& sc,
        const float& ace,
        const float& dce,
        const float& sce,
        const float& se,
        const bool& g
    );
};

#include "SurfaceProperties.inl"