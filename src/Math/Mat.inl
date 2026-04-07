#include "Mat.h"

// Mat3 Struct

constexpr Mat3::Mat3(const Vec3& u, const Vec3& v, const Vec3& w)
                    : u(u), v(v), w(w) {}

constexpr Mat3::Mat3(const Mat4& mat4)
                    : u(Vec3(mat4.x)), v(Vec3(mat4.y)), w(Vec3(mat4.z)) {}

inline Mat3 Mat3::inverse() const
{
    Vec3 r0 = v.cross(w);
    Vec3 r1 = w.cross(u);
    Vec3 r2 = u.cross(v);

    float invDet = 1.0f / u.dot(r0); // Assume invertible (no checks)

    return Mat3(Vec3(r0.x, r1.x, r2.x),
                Vec3(r0.y, r1.y, r2.y),
                Vec3(r0.z, r1.z, r2.z))
           * invDet;
}

inline Mat3 Mat3::transpose() const
{
    return Mat3(Vec3(u.x, v.x, w.x),
                Vec3(u.y, v.y, w.y),
                Vec3(u.z, v.z, w.z));
}

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

inline Mat3 Mat3::operator*(const float& scalar) const
{
    return Mat3(u * scalar,
                v * scalar,
                w * scalar);
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
                    : x(x), y(y), z(z), w(w) {}

constexpr Mat4::Mat4(const Mat3& mat3)
                    : x(Vec4(mat3.u)), y(Vec4(mat3.v)), z(Vec4(mat3.w)), w(Vec4(0, 0, 0, 1)) {}

inline Mat4 Mat4::identity()
{
    Mat4 identity;

    identity.x.x = 1;
    identity.y.y = 1;
    identity.z.z = 1;
    identity.w.w = 1;

    return identity;
}

inline Mat4 Mat4::inverseModel() const
{
    Mat4 inverseModel;
    Mat3 R = Mat3(*this).inverse();

    inverseModel.x = Vec4(R.u);
    inverseModel.y = Vec4(R.v);
    inverseModel.z = Vec4(R.w);
    inverseModel.w = Vec4(-(R.matvec(Vec3(w))));
    inverseModel.w.w = 1;

    return inverseModel;
}

inline Mat3 Mat4::normalMatrix() const
{
    return Mat3(*this).inverse().transpose();
}

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

inline Mat4 Mat4::matmat(const Mat4& otherM) const
{
    Mat3 newTopLeft = Mat3(*this).matmat(Mat3(otherM));
    Vec4 newTranslation = Vec4(Vec3(this->w) + Vec3(otherM.w));

    Mat4 newMat = Mat4(newTopLeft);
    newMat.w = newTranslation;

    return newMat;
}

inline Mat4 Mat4::operator-() const
{
    return Mat4(x, y, z, -w);
}