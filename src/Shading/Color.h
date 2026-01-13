#pragma once

#include "Math/Math.h"
#include <algorithm>

// Linear between [0, 1], Gamma Correct
struct Color {
    float r, g, b;

    Color(const float& r = 0, const float& g = 0, const float& b = 0);

    Color operator*(const float& coeff) const;

    Color operator/(const float& coeff) const;

    Color operator*(const Color& otherColor) const;

    Color operator+(const Color& otherColor) const;

    void toSRGB();

    void clamp();
};

#include "Color.inl"