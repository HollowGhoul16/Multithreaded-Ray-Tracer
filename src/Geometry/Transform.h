#pragma once

#include "Math/Math.h"

struct Transform {
    Mat4 modelMatrix;
    Mat4 inverseMatrix;
    Mat3 normalMatrix;

    Transform() = default;

    Transform(const Mat4& modelMatrix);

    Ray rayToLocal(const Ray& worldRay) const;

    Vec3 pointToWorld(const Vec3& localPoint) const;

    Vec3 normalToWorld(const Vec3& localNormal) const;

    void update();
};

#include "Transform.inl"