#include "Lights.h"

// Light Struct

inline Light::Light(const Color& c, const float& i, const Type& t)
                   : color(c), intensity(i), type(t) {}

// DirectionalLight struct

inline DirectionalLight::DirectionalLight(const Color& c, const Vec3& d, const float& i)
                                         : Light(c, i, Type::Directional), direction(d.normalize()) {}

inline float DirectionalLight::calculateIntensity(const Vec3& point) const
{
    return intensity;
}

inline Vec3 DirectionalLight::getDirection(const Vec3& point) const
{
    return direction;
}

inline float DirectionalLight::distanceTo(const Vec3& point) const
{
    return std::numeric_limits<float>::infinity();
}

// PointLight struct

inline PointLight::PointLight(const Color& c, const Vec3& o, const float& i, Surface* s)
                             : Light(c, i, Type::Point), origin(o), surface(s) {}

inline float PointLight::calculateIntensity(const Vec3& point) const
{
    float distance = distanceTo(point);
    return intensity / (distance * distance);
}

inline Vec3 PointLight::getDirection(const Vec3& point) const
{
    return (point - origin).normalize();
}

inline float PointLight::distanceTo(const Vec3& point) const
{
    return (point - origin).magnitude();
}