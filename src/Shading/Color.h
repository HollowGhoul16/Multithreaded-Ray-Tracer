#pragma once

#include "Math/Math.h"
#include <algorithm>

// Linear between [0, 1], Gamma Correct
struct Color {
    float r, g, b;

    Color();

    Color(const float& r, const float& g, const float& b);

    static Color lerp(const Color& color1, const Color& color2, const float& scalar);

    static float smoothStep(const float& min, const float& max, const float& value);

    void toSRGB();

    void clamp();

    Color operator*(const float& coeff) const;

    Color operator/(const float& coeff) const;

    Color operator*(const Color& otherColor) const;

    Color operator+(const Color& otherColor) const;
};

#include "Color.inl"