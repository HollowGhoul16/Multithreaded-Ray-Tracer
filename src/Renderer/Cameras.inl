#include "Cameras.h"

// Camera abstract struct

inline Camera::Camera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation)
                     : origin(origin)
{
    basis.w = (lookAt - origin).normalize();
    basis.u = basis.w.cross(orientation).normalize();
    basis.v = basis.u.cross(basis.w);
}

inline void Camera::updateOrigin(const Vec3& shift)
{
    origin = origin + shift;
}

inline void Camera::yaw(const float& theta)
{
    Matrix3 rotation(Vec3(std::cos(theta), 0, -std::sin(theta)), Vec3(0, 1, 0), Vec3(std::sin(theta), 0, std::cos(theta)));
    Matrix3 newBasis = basis.matmat(rotation);
    newBasis.orthoNormalize();
    basis = newBasis;
}

inline void Camera::pitch(const float& theta)
{
    Matrix3 rotation(Vec3(1, 0, 0), Vec3(0, std::cos(theta), std::sin(theta)), Vec3(0, -std::sin(theta), std::cos(theta)));
    Matrix3 newBasis = basis.matmat(rotation);
    newBasis.orthoNormalize();
    basis = newBasis;
}

inline void Camera::roll(const float& theta)
{
    Matrix3 rotation(Vec3(std::cos(theta), std::sin(theta), 0), Vec3(-std::sin(theta), std::cos(theta), 0), Vec3(0, 0, 1));
    Matrix3 newBasis = basis.matmat(rotation);
    newBasis.orthoNormalize();
    basis = newBasis;
}

// OrthographicCamera struct

inline OrthographicCamera::OrthographicCamera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation)
                                             : Camera(origin, lookAt, orientation) {}

inline Ray OrthographicCamera::getRay(const float& x, const float& y) const
{
    return Ray(origin + basis.u * x + basis.v * y, basis.w);
}

// PerspectiveCamera struct

inline PerspectiveCamera::PerspectiveCamera(const Vec3& origin, const Vec3& lookAt, const Vec3& orientation, const float d)
                                           : Camera(origin, lookAt, orientation), distToImage(d) {}

inline Ray PerspectiveCamera::getRay(const float& x, const float& y) const
{
    return Ray(origin, (basis.w * distToImage + basis.u * x + basis.v * y).normalize());
}