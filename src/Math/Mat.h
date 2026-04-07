#pragma once

#include "Vec.h"

struct Mat4;

struct Mat3 {
    Vec3 u, v, w;

    constexpr Mat3() = default;

    constexpr Mat3(const Vec3& u, const Vec3& v, const Vec3& w);

    constexpr Mat3(const Mat4& mat4);

    Mat3 inverse() const;

    Mat3 transpose() const;

    Vec3 matvec(const Vec3& vec) const;

    Mat3 matmat(const Mat3& otherM) const;

    Mat3 operator*(const float& scalar) const;

    void orthoNormalize(); // Prevent floating point errors for rotations with camera
};

struct Mat4 {
    Vec4 x, y, z, w;

    constexpr Mat4() = default;

    constexpr Mat4(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w);

    constexpr Mat4(const Mat3& mat3);

    static Mat4 identity();

    Mat4 inverseModel() const;

    Mat3 normalMatrix() const; // For transforming normal vectors

    Vec3 matvec(const Vec3& vec) const; // Uses 1 for w value for use of homogeneous coordinates

    Vec4 matvec(const Vec4& vec) const;

    Mat4 matmat(const Mat4& otherM) const;

    Mat4 operator-() const;
};

#include "Mat.inl"