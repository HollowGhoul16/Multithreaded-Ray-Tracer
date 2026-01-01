#pragma once

#include <math.h>

struct Vec3 {
    float x, y, z;

    constexpr Vec3();

    constexpr Vec3(const float& x, const float& y, const float& z);

    const float& operator[](const int& i) const;

    Vec3 operator*(const float& scalar) const;

    Vec3 operator/(const float& scalar) const;

    Vec3 operator+(const Vec3& otherVec) const;

    Vec3 operator-(const Vec3& otherVec) const;

    Vec3 operator-() const;

    float magnitude() const;

    Vec3 normalize() const;

    float dot(const Vec3& otherVec) const;

    Vec3 cross(const Vec3& otherVec) const;
};

struct Matrix3 {
    Vec3 u, v, w;

    constexpr Matrix3();

    constexpr Matrix3(const Vec3& u, const Vec3& v, const Vec3& w);

    Vec3 matvec(const Vec3& vec) const;

    Matrix3 matmat(const Matrix3& otherM) const;

    void orthoNormalize(); // Prevent floating point errors for rotations with camera
};

struct Ray {
    Vec3 origin, direction;

    constexpr Ray(const Vec3& o, const Vec3& d);

    Vec3 parametrize(const float& t) const;
};

#include "Math.inl"