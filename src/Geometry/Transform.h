#pragma once

#include "Math/Math.h"

struct Transform {
    Mat4 modelMatrix;
    Mat4 inverseMatrix;
    Mat3 normalMatrix;

    Transform() = default;

    Transform(const Mat4& modelMatrix);

    Ray rayToLocal(const Ray& worldRay) const;

    Vec3 pointToLocal(const Vec3& worldPoint) const;

    float tToWorld(const Ray& worldRay, const Vec3& localPointHit) const;

    Vec3 pointToWorld(const Vec3& localPoint) const;

    Vec3 normalToWorld(const Vec3& localNormal) const;

    void applyTransform(const Mat4& transformation);

    void update();
};

#include "Transform.inl"