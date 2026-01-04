#include "Lights.h"

// Light Struct

constexpr Light::Light(const float& i)
                      : intensity(i) {};

// DirectionalLight struct

inline DirectionalLight::DirectionalLight(const Vec3& d, const float& i)
                                         : Light(i), direction(d.normalize()) {};

inline float DirectionalLight::calculateIntensity(const Vec3& point) const
{
    return intensity;
}

// PointLight struct

constexpr PointLight::PointLight(const Vec3& o, const float& i)
                                : Light(i), origin(o) {};

inline float PointLight::calculateIntensity(const Vec3& point) const
{
    float distance = (origin - point).magnitude();
    return intensity / (distance * distance);
}