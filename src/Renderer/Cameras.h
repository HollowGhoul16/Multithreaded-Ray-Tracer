#pragma once

#include "Math/Math.h"

struct Camera {
    Vec3 origin;
    Matrix3 basis; // u = right, v = up, w = lookAt (vectors)

    Camera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation);

    virtual ~Camera() = default;

    virtual Ray getRay(const float& x, const float& y) const = 0;

    void updateOrigin(const Vec3& shift);

    void pitch(const float& theta);

    void yaw(const float& theta);

    void roll(const float& theta);
};

struct OrthographicCamera : Camera {
    OrthographicCamera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation);

    Ray getRay(const float& x, const float& y) const override;
};

struct PerspectiveCamera : Camera {
    float distToImage; // Determines FOV

    PerspectiveCamera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation, const float d);

    Ray getRay(const float& x, const float& y) const override;
};

#include "Cameras.inl"