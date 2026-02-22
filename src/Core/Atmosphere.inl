#include "Atmosphere.h"

inline Atmosphere::Atmosphere(
    const DirectionalLight& sunDir,
    const float& sunSize,
    const Color& skyColor,
    const Color& horColor)
    : sun(sunDir),
      sunSize(sunSize),
      skyColor(skyColor),
      horizonColor(horColor)
{}

inline Color Atmosphere::skyModel(const Ray& ray) const
{
    Vec3 rayDir = ray.direction;
    Vec3 sunDir = -sun.direction;
    float cosTheta = std::max(rayDir.dot(sunDir), 0.0f);

    float horizonScalar = -1 * (Vec3(0.0f, 1.0f, 0.0f).dot(rayDir) - 1); // Scale based off of ray's y-value
    horizonScalar = std::pow(horizonScalar, 5.0f);

    float skyGradient = 1 + cosTheta * cosTheta;
    Color skyBlend = (skyColor * skyGradient) * (1 - horizonScalar) + horizonColor * horizonScalar;

    float sunScalar = 0.05f + std::pow(cosTheta, 128.0f / sunSize);
    Color sunBlend = sun.color * sunScalar * 2.0f;

    return skyBlend * (1 - sunScalar) + sunBlend * sunScalar;
}