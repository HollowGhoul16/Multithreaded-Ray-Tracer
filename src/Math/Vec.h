#pragma once

#include <math.h>
#include <random>

struct Vec4;

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

    constexpr Vec3(const Vec4& vec4);

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

#include "Vec.inl"