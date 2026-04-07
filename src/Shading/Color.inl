#include "Color.h"

// Color struct

inline Color::Color() : r(0), g(0), b(0) {}

inline Color::Color(const float& r, const float& g, const float& b)
                   : r(std::pow(r / 255, 2.2)),
                     g(std::pow(g / 255, 2.2)),
                     b(std::pow(b / 255, 2.2))
                   {}

inline Color Color::lerp(const Color& color1, const Color& color2, const float& scalar)
{
    return color1 * (1 - scalar) + color2 * scalar;
}

inline float Color::smoothStep(const float& min, const float& max, const float& value)
{
    float t = std::min(std::max((value - min) / (max - min), 0.0f), 1.0f);
    return t * t * (3 - 2 * t);
}

// Go from linear [0, 1] range to [0, 255] for image display
inline void Color::toSRGB()
{
    r = 255 * std::pow(r, .454);
    g = 255 * std::pow(g, .454);
    b = 255 * std::pow(b, .454);
}

inline void Color::clamp() 
{
    r = std::min(r, 1.0f);
    r = std::max(r, 0.0f);
    g = std::min(g, 1.0f);
    g = std::max(g, 0.0f);
    b = std::min(b, 1.0f);
    b = std::max(b, 0.0f);
}

inline Color Color::operator*(const float& coeff) const 
{
    Color color;
    color.r = r * coeff;
    color.g = g * coeff;
    color.b = b * coeff;
    return color;
}

inline Color Color::operator/(const float& coeff) const 
{
    Color color;
    color.r = r / coeff;
    color.g = g / coeff;
    color.b = b / coeff;
    return color;
}

inline Color Color::operator-(const float& val) const
{
    Color color;
    color.r = r - val;
    color.g = g - val;
    color.b = b - val;
    return color;
}

inline Color Color::operator*(const Color& otherColor) const 
{
    Color color;
    color.r = r * otherColor.r;
    color.g = g * otherColor.g;
    color.b = b * otherColor.b;
    return color;
}

inline Color Color::operator+(const Color& otherColor) const 
{
    Color color;
    color.r = r + otherColor.r;
    color.g = g + otherColor.g;
    color.b = b + otherColor.b;
    return color;
}