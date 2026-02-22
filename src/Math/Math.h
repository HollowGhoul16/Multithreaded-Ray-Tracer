#pragma once

#include <math.h>
#include <random>

struct Vec2 {
    float x, y;

    constexpr Vec2();

    constexpr Vec2(const float& x, const float& y);

    static Vec2 randomRayOffset();

    float magnitude() const;
};

struct Vec3 {
    float x, y, z;

    constexpr Vec3();

    constexpr Vec3(const float& value);

    constexpr Vec3(const float& x, const float& y, const float& z);

    static Vec3 randomSphereUnitVector();

    float magnitude() const;

    Vec3 normalize() const;

    float dot(const Vec3& otherVec) const;

    Vec3 cross(const Vec3& otherVec) const;

    const float& operator[](const int& i) const;

    float& operator[](const int& i);

    Vec3 operator*(const float& scalar) const;

    Vec3 operator/(const float& scalar) const;

    Vec3 operator+(const Vec3& otherVec) const;

    Vec3 operator-(const Vec3& otherVec) const;

    Vec3 operator-() const;
};

Vec3 operator/(const float& scalar, const Vec3& vec);

struct Vec4 {
    float x, y, z, w;

    constexpr Vec4();

    constexpr Vec4(const float& value);

    constexpr Vec4(const float& x, const float& y, const float& z, const float& w);

    float magnitude() const;

    Vec4 normalize() const;

    float dot(const Vec4& otherVec) const;

    const float& operator[](const int& i) const;

    float& operator[](const int& i);

    Vec4 operator*(const float& scalar) const;

    Vec4 operator/(const float& scalar) const;

    Vec4 operator+(const Vec4& otherVec) const;

    Vec4 operator-(const Vec4& otherVec) const;

    Vec4 operator-() const;
};

struct Matrix3 {
    Vec3 u, v, w;

    constexpr Matrix3() = default;

    constexpr Matrix3(const Vec3& u, const Vec3& v, const Vec3& w);

    Vec3 matvec(const Vec3& vec) const;

    Matrix3 matmat(const Matrix3& otherM) const;

    void orthoNormalize(); // Prevent floating point errors for rotations with camera
};

struct Matrix4 {
    Vec4 x, y, z, w;

    constexpr Matrix4() = default;

    constexpr Matrix4(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w);

    Vec3 matvec(const Vec3& vec) const; // Uses 1 for w value for use of homogeneous coordinates

    Vec4 matvec(const Vec4& vec) const;
};

struct Ray {
    Vec3 origin, direction;

    Ray() = default;

    Ray(const Vec3& o, const Vec3& d);

    Vec3 parametrize(const float& t) const;
};

#include "Math.inl"