#include "Mat.h"

// Mat3 Struct

constexpr Mat3::Mat3(const Vec3& u, const Vec3& v, const Vec3& w)
                    : u(u), v(v), w(w) {};

inline Vec3 Mat3::matvec(const Vec3& vec) const 
{
    Vec3 col1 = u * vec.x;
    Vec3 col2 = v * vec.y;
    Vec3 col3 = w * vec.z;

    return Vec3(col1.x + col2.x + col3.x, 
                col1.y + col2.y + col3.y, 
                col1.z + col2.z + col3.z);
}

inline Mat3 Mat3::matmat(const Mat3& otherM) const 
{
    return Mat3(matvec(otherM.u), matvec(otherM.v), matvec(otherM.w));
}

inline void Mat3::orthoNormalize() 
{
    u = u.normalize();
    v = v - (u * v.dot(u));
    v = v.normalize();
    w = v.cross(u);
    w = w.normalize();
}

// Mat4 Struct

constexpr Mat4::Mat4(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w)
                    : x(x), y(y), z(z), w(w) {};

inline Vec3 Mat4::matvec(const Vec3& vec) const 
{
    Vec4 col1 = x * vec.x;
    Vec4 col2 = y * vec.y;
    Vec4 col3 = z * vec.z;
    Vec4 col4 = w;

    return Vec3(col1.x + col2.x + col3.x + col4.x,
                col1.y + col2.y + col3.y + col4.y,
                col1.z + col2.z + col3.z + col4.z);
}

inline Vec4 Mat4::matvec(const Vec4& vec) const 
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