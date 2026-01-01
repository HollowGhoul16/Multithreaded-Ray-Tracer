#pragma once

#include "Math/Math.h"
#include "Lights.h"
#include <algorithm>

struct Color {
    float r, g, b;

    constexpr Color(const float& r = 0, const float& g = 0, const float& b = 0);

    Color operator*(const float& coeff) const;

    Color operator/(const float& coeff) const;

    Color operator*(const Color& otherColor) const;

    Color operator+(const Color& otherColor) const;

    void clamp();
};

// Sum of the coefficients should between 0 and 1 inclusively
struct Material {
    Color ambientColor, diffuseColor, specularColor;
    float ambientCoeff, diffuseCoeff, specularCoeff;
    float specularExp;
    bool glazed;

    constexpr Material(
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