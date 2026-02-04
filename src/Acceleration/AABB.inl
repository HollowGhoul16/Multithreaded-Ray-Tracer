#include "AABB.h"

inline std::pair<bool, float> AABB::intersection(const Ray &ray, const float& tClosestMax) const
{
    Vec3 invDir = 1.0f / ray.direction;

    // Determine which bounds to use
    int sign[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};

    float tmin = ((sign[0] ? max.x : min.x) - ray.origin.x) * invDir.x;
    float tmax = ((sign[0] ? min.x : max.x) - ray.origin.x) * invDir.x;

    float tymin = ((sign[1] ? max.y : min.y) - ray.origin.y) * invDir.y;
    float tymax = ((sign[1] ? min.y : max.y) - ray.origin.y) * invDir.y;

    if(tmin > tymax || tymin > tmax) return {false, 0.0f};

    if(tymin > tmin) tmin = tymin;
    if(tymax < tmax) tmax = tymax;

    float tzmin = ((sign[2] ? max.z : min.z) - ray.origin.z) * invDir.z;
    float tzmax = ((sign[2] ? min.z : max.z) - ray.origin.z) * invDir.z;

    if(tmin > tzmax || tzmin > tmax) return {false, 0.0f};

    if(tzmin > tmin) tmin = tzmin;
    if(tzmax < tmax) tmax = tzmax;

    if(tmax < 0) return {false, 0.0f}; // box is behind the ray

    float tHit = (tmin >= 0) ? tmin : tmax;

    if(tHit <= tClosestMax) return {true, tHit};
    else return {false, 0.0f};
}