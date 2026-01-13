#include "Lights.h"

// Light Struct

inline Light::Light(const Color& c, const float& i)
                   : color(c), intensity(i) {};

// DirectionalLight struct

inline DirectionalLight::DirectionalLight(const Color& c, const Vec3& d, const float& i)
                                         : Light(c, i), direction(d.normalize()) {};

inline float DirectionalLight::calculateIntensity(const Vec3& point) const
{
    return intensity;
}

// PointLight struct

inline PointLight::PointLight(const Color& c, const Vec3& o, const float& i)
                             : Light(c, i), origin(o) {};

inline float PointLight::calculateIntensity(const Vec3& point) const
{
    float distance = (origin - point).magnitude();
    return intensity / (distance * distance);
}