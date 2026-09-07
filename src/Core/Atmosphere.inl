#include "Atmosphere.h"

inline Atmosphere::Atmosphere(
    const DirectionalLight& luminaryDir,
    const float& luminarySize,
    const Color& skyColor,
    const Color& horColor,
    const bool& isSun)
    : luminary(luminaryDir)
{
    sky = Procedural(luminarySize, skyColor, horColor);
    selectedModel = isSun ? &Atmosphere::sunModel : &Atmosphere::moonModel;
}

inline Atmosphere::Atmosphere(
    const DirectionalLight& luminaryDir,
    const Cubemap& cubeMap)
    : luminary(luminaryDir)
{
    sky = cubeMap;
    selectedModel = &Atmosphere::skybox;
}

inline Color Atmosphere::skyModel(const Ray& ray) const
{
    return (this->*selectedModel)(ray);
}

inline Color Atmosphere::skybox(const Ray& ray) const
{
    return std::get<Cubemap>(sky).sample(ray.direction);
}

inline Color Atmosphere::sunModel(const Ray& ray) const
{
    Vec3 rayDir = ray.direction;
    Vec3 sunDir = -luminary.direction;
    float cosTheta = std::max(rayDir.dot(sunDir), 0.0f);

    float horizonScalar = -1 * (std::max(Vec3(0.0f, 1.0f, 0.0f).dot(rayDir), 0.0f) - 1); // Scale based off of ray's y-value
    horizonScalar = std::pow(horizonScalar, 4.0f);

    float skyGradient = 1 + cosTheta * cosTheta;
    Color skyBlend = Color::lerp((std::get<Procedural>(sky).skyColor * skyGradient), std::get<Procedural>(sky).horizonColor, horizonScalar);

    float sunScalar = 0.05f + std::pow(cosTheta, 128.0f / std::get<Procedural>(sky).luminarySize);
    Color sunBlend = luminary.color * sunScalar * 2.0f;

    return Color::lerp(skyBlend, sunBlend, sunScalar);
}

// TODO: Figure out sky flickering
inline Color Atmosphere::moonModel(const Ray& ray) const
{
    Vec3 rayDir = ray.direction;
    Vec3 moonDir = -luminary.direction;
    float cosTheta = std::max(rayDir.dot(moonDir), 0.0f);
    bool hitMoon = (cosTheta >= std::sin((std::get<Procedural>(sky).luminarySize / .18f)));

    if(hitMoon) {
        Color moonColor = luminary.color * std::pow(cosTheta, 200.0f);
        return moonColor;
    }
    else {
        float horizonScalar = -1 * (std::max(Vec3(0.0f, 1.0f, 0.0f).dot(rayDir), 0.0f) - 1); // Scale based off of ray's y-value
        horizonScalar = std::pow(horizonScalar, 4.0f);

        Color skyBlend = Color::lerp(std::get<Procedural>(sky).skyColor, std::get<Procedural>(sky).horizonColor, horizonScalar);

        float moonBlend = std::pow(Color::smoothStep(.8, 1.2, cosTheta), 4);

        return Color::lerp(skyBlend, luminary.color, moonBlend);
    }
}