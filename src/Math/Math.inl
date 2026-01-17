#include "Math.h"

// Vec3 Struct

constexpr Vec3::Vec3() : x(0), y(0), z(0) {};

constexpr Vec3::Vec3(const float& x, const float& y, const float& z)
                    : x(x), y(y), z(z) {};

inline float Vec3::magnitude() const 
{
    return std::sqrt(x * x +
                     y * y +
                     z * z); 
}

inline Vec3 Vec3::normalize() const 
{
    if(this->magnitude() == 0) return Vec3(0, 0, 0);
    return (*this / this->magnitude()); 
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

// Matrix3 Struct

constexpr Matrix3::Matrix3() {};

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

// Ray Struct

constexpr Ray::Ray(const Vec3& o, const Vec3& d)
                  : origin(o), direction(d) {};

inline Vec3 Ray::parametrize(const float& t) const 
{
    return origin + direction * t;
}