#pragma once

#include "Math/Vec.h"
#include "Color.h"

struct Surface;

struct Light {
    Color color;
    float intensity;

    enum class Type {
        Directional,
        Point
    };

    Type type;

    Light(const Color& c, const float& i, const Type& t);

    virtual ~Light() = default;

    virtual float calculateIntensity(const Vec3& point) const = 0;

    virtual Vec3 getDirection(const Vec3& point) const = 0;

    virtual float distanceTo(const Vec3& point) const = 0;
};

struct DirectionalLight : Light {
    Vec3 direction;

    DirectionalLight(const Color& c, const Vec3& d, const float& i);

    float calculateIntensity(const Vec3& point) const override;

    Vec3 getDirection(const Vec3& point) const override;

    float distanceTo(const Vec3& point) const override;
};

struct PointLight : Light {
    Vec3 origin;
    Surface* surface;

    PointLight(const Color& c, const Vec3& o, const float& i, Surface* s);

    float calculateIntensity(const Vec3& point) const override;

    Vec3 getDirection(const Vec3& point) const override;

    float distanceTo(const Vec3& point) const override;
};

#include "Lights.inl"