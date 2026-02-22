#include "Math.h"

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

// Vec2 Free functions

// Vec3 Struct

constexpr Vec3::Vec3() : x(0), y(0), z(0) {}

constexpr Vec3::Vec3(const float& value)
                    : x(value), y(value), z(value) {}

constexpr Vec3::Vec3(const float& x, const float& y, const float& z)
                    : x(x), y(y), z(z) {}

inline Vec3 Vec3::randomSphereUnitVector()
{
    static const int ITERATE_LIMIT = 100;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

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
    if(magnitude < 1e-9f) return Vec3(0, 0, 0);
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
    if(magnitude < 1e-9f) return Vec4(0, 0, 0, 0);
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

inline Vec4 operator/(const float& scalar, const Vec4& vec)
{
    return Vec4(scalar / vec.x,
                scalar / vec.y,
                scalar / vec.z,
                scalar / vec.w);
}

// Matrix3 Struct

constexpr Matrix3::Matrix3(const Vec3& u, const Vec3& v, const Vec3& w)
                          : u(u), v(v), w(w) {};

inline Vec3 Matrix3::matvec(const Vec3& vec) const 
{
    Vec3 col1 = u * vec.x;
    Vec3 col2 = v * vec.y;
    Vec3 col3 = w * vec.z;

    return Vec3(col1.x + col2.x + col3.x, 
                col1.y + col2.y + col3.y, 
                col1.z + col2.z + col3.z);
}

inline Matrix3 Matrix3::matmat(const Matrix3& otherM) const 
{
    return Matrix3(matvec(otherM.u), matvec(otherM.v), matvec(otherM.w));
}

inline void Matrix3::orthoNormalize() 
{
    u = u.normalize();
    v = v - (u * v.dot(u));
    v = v.normalize();
    w = v.cross(u);
    w = w.normalize();
}

// Matrix4 Struct

constexpr Matrix4::Matrix4(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w)
                          : x(x), y(y), z(z), w(w) {};

inline Vec3 Matrix4::matvec(const Vec3& vec) const 
{
    Vec4 col1 = x * vec.x;
    Vec4 col2 = y * vec.y;
    Vec4 col3 = z * vec.z;
    Vec4 col4 = w;

    return Vec3(col1.x + col2.x + col3.x + col4.x,
                col1.y + col2.y + col3.y + col4.y,
                col1.z + col2.z + col3.z + col4.z);
}

inline Vec4 Matrix4::matvec(const Vec4& vec) const 
{
    Vec4 col1 = x * vec.x;
    Vec4 col2 = y * vec.y;
    Vec4 col3 = z * vec.z;
    Vec4 col4 = w * vec.w;

    return Vec4(col1.x + col2.x + col3.x + col4.x,
                col1.y + col2.y + col3.y + col4.y, 
                col1.z + col2.z + col3.z + col4.z,
                col1.w + col2.w + col3.w + col4.w);
}

// Ray Struct

inline Ray::Ray(const Vec3& o, const Vec3& d)
               : origin(o), direction(d) {};

inline Vec3 Ray::parametrize(const float& t) const 
{
    return origin + direction * t;
}