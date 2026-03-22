#include "Transform.h"

inline Transform::Transform(const Mat4& modelMatrix)
                           : modelMatrix(modelMatrix)
{
    update();
}

inline Ray Transform::rayToLocal(const Ray& worldRay) const
{
    Ray localRay;
    localRay.origin = inverseMatrix.matvec(worldRay.origin);
    localRay.direction = Mat3(inverseMatrix).matvec(worldRay.direction);

    return localRay;
}

inline Vec3 Transform::pointToWorld(const Vec3& localPoint) const
{
    return modelMatrix.matvec(localPoint);
}

inline Vec3 Transform::normalToWorld(const Vec3& localNormal) const
{
    return normalMatrix.matvec(localNormal).normalize();
}

inline void Transform::update()
{
    inverseMatrix = modelMatrix.inverseModel();
    normalMatrix = modelMatrix.normalMatrix();
}
