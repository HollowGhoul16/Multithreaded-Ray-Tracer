#pragma once

#include "Color.h"
#include "Math/Math.h"
#include "Lights.h"

// Sum of the coefficients should be between [0, 1]
struct Material {
    Color ambientColor, diffuseColor, specularColor;
    float ambientCoeff, diffuseCoeff, specularCoeff;
    float specularExp;
    bool isGlazed, isMirror;

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

    Color getColor(
        const Ray& ray, 
        const Vec3& normal, 
        const DirectionalLight& light,
        const bool& shadow
    ) const;
};

#include "SurfaceProperties.inl"