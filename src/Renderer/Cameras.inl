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

inline void Camera::applyTransform(const Mat4& transform)
{
    basis = basis.matmat(transform);
    basis.orthoNormalize();
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