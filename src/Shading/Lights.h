#pragma once

#include "Math/Math.h"
#include "Color.h"

struct Light {
    Color color;
    float intensity;

    Light(const Color& c, const float& i);

    virtual ~Light() = default;

    virtual float calculateIntensity(const Vec3& point) const = 0;
};

struct DirectionalLight : Light {
    Vec3 direction;

    DirectionalLight(const Color& c, const Vec3& d, const float& i);

    float calculateIntensity(const Vec3& point) const override;
};

struct PointLight : Light {
    Vec3 origin;

    PointLight(const Color& c, const Vec3& o, const float& i);

    float calculateIntensity(const Vec3& point) const override;
};

#include "Lights.inl"