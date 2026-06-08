#include "Vec.h"

// Vec2 Struct

constexpr Vec2::Vec2() : x(0), y(0) {}

constexpr Vec2::Vec2(const float& x, const float& y)
                    : x(x), y(y) {}

inline Vec2 Vec2::randomRayOffset()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.5f, 0.5f);

    return Vec2(dist(gen), dist(gen));
}

inline float Vec2::magnitude() const
{
    return std::sqrt(x * x +
                     y * y);
}

inline float Vec2::cross(const Vec2& otherVec) const
{
    return (this->x * otherVec.y) - (this->y * otherVec.x);
}

inline Vec2 Vec2::operator*(const float& scalar) const
{
    return Vec2(x * scalar,
                y * scalar);
}

inline Vec2 Vec2::operator+(const Vec2& otherVec) const
{
    return Vec2(x + otherVec.x,
                y + otherVec.y);
}

inline Vec2 Vec2::operator-(const Vec2& otherVec) const
{
    return Vec2(x - otherVec.x,
                y - otherVec.y);
}

// Vec3 Struct

constexpr Vec3::Vec3() : x(0), y(0), z(0) {}

constexpr Vec3::Vec3(const float& value)
                    : x(value), y(value), z(value) {}

constexpr Vec3::Vec3(const float& x, const float& y, const float& z)
                    : x(x), y(y), z(z) {}

constexpr Vec3::Vec3(const Vec4& vec4)
                    : x(vec4.x), y(vec4.y), z(vec4.z) {}

inline Vec3 Vec3::randomSphereUnitVector()
{
    static const int ITERATE_LIMIT = 100;
    static std::random_device rd;
    thread_local std::mt19937 gen(rd());
    thread_local std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    Vec3 randVec = Vec3(dist(gen), dist(gen), dist(gen));

    for(int i = 0; i < ITERATE_LIMIT; ++i) {
        if(randVec.magnitude() <= 1) return randVec.normalize();
        randVec = Vec3(dist(gen), dist(gen), dist(gen));
    }

    return Vec3(); // Return the zero vector if we are unlucky
}

inline float Vec3::magnitude() const
{
    return std::sqrt(x * x +
                     y * y +
                     z * z);
}

inline Vec3 Vec3::normalize() const
{
    float magnitude = this->magnitude();
    if(magnitude < Math::EPSILON) return Vec3(0, 0, 0);
    return (*this / magnitude);
}

inline float Vec3::dot(const Vec3& otherVec) const
{
    return (this->x * otherVec.x +
            this->y * otherVec.y +
            this->z * otherVec.z);
}

inline Vec3 Vec3::cross(const Vec3& otherVec) const
{
    return Vec3((this->y * otherVec.z - this->z * otherVec.y),
                (this->z * otherVec.x - this->x * otherVec.z),
                (this->x * otherVec.y - this->y * otherVec.x));
}

inline const float& Vec3::operator[](const int& i) const
{
    return i == 0 ? x : (i == 1 ? y : z);
}

inline float& Vec3::operator[](const int& i)
{
    return i == 0 ? x : (i == 1 ? y : z);
}

inline Vec3 Vec3::operator*(const float& scalar) const
{
    return Vec3(x * scalar,
                y * scalar,
                z * scalar);       
}

inline Vec3 Vec3::operator/(const float& scalar) const
{
    return Vec3(x / scalar,
                y / scalar,
                z / scalar);       
}

inline Vec3 Vec3::operator+(const Vec3& otherVec) const
{
    return Vec3(x + otherVec.x,
                y + otherVec.y,
                z + otherVec.z); 
}

inline Vec3 Vec3::operator-(const Vec3& otherVec) const
{
    return Vec3(x - otherVec.x,
                y - otherVec.y,
                z - otherVec.z); 
}

inline Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);          
}

// Free functions for Vec3

inline Vec3 operator/(const float& scalar, const Vec3& vec)
{
    return Vec3(scalar / vec.x,
                scalar / vec.y,
                scalar / vec.z);
}

// Vec4 Struct

constexpr Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

constexpr Vec4::Vec4(const float& value)
                    : x(value), y(value), z(value), w(value) {}

constexpr Vec4::Vec4(const float& x, const float& y, const float& z, const float& w)
                    : x(x), y(y), z(z), w(w) {}

constexpr Vec4::Vec4(const Vec3& vec3)
                    : x(vec3.x), y(vec3.y), z(vec3.z), w(0) {}

inline float Vec4::magnitude() const 
{
    return std::sqrt(x * x +
                     y * y +
                     z * z +
                     w * w);
}

inline Vec4 Vec4::normalize() const 
{
    float magnitude = this->magnitude();
    if(magnitude < Math::EPSILON) return Vec4(0, 0, 0, 0);
    return (*this / magnitude); 
}

inline float Vec4::dot(const Vec4& otherVec) const 
{
    return (this->x * otherVec.x +
            this->y * otherVec.y +
            this->z * otherVec.z +
            this->w * otherVec.w);
}

inline const float& Vec4::operator[](const int& i) const 
{
    return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w));
}

inline float& Vec4::operator[](const int& i)
{
    return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w));
}

inline Vec4 Vec4::operator*(const float& scalar) const 
{
    return Vec4(x * scalar,
                y * scalar,
                z * scalar,
                w * scalar);       
}

inline Vec4 Vec4::operator/(const float& scalar) const 
{
    return Vec4(x / scalar,
                y / scalar,
                z / scalar,
                w / scalar);       
}

inline Vec4 Vec4::operator+(const Vec4& otherVec) const 
{
    return Vec4(x + otherVec.x,
                y + otherVec.y,
                z + otherVec.z,
                w + otherVec.w); 
}

inline Vec4 Vec4::operator-(const Vec4& otherVec) const 
{
    return Vec4(x - otherVec.x,
                y - otherVec.y,
                z - otherVec.z,
                w - otherVec.w); 
}

inline Vec4 Vec4::operator-() const 
{
    return Vec4(-x, -y, -z, -w);          
}

// Free functions for Vec4

inline Vec4 operator/(const float& scalar, const Vec4& vec) // Prevents division by 0
{
    return Vec4(vec.x ? scalar / vec.x : 0,
                vec.y ? scalar / vec.y : 0,
                vec.z ? scalar / vec.z : 0,
                vec.w ? scalar / vec.w : 0);
}