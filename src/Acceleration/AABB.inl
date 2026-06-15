#include "AABB.h"

inline HitData AABB::intersection(const Ray &ray) const
{
    HitData hitData;

    Vec3 invDir = 1.0f / ray.direction;

    // Determine which bounds to use
    int sign[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};

    float tmin = ((sign[0] ? max.x : min.x) - ray.origin.x) * invDir.x;
    float tmax = ((sign[0] ? min.x : max.x) - ray.origin.x) * invDir.x;

    float tymin = ((sign[1] ? max.y : min.y) - ray.origin.y) * invDir.y;
    float tymax = ((sign[1] ? min.y : max.y) - ray.origin.y) * invDir.y;

    if(tmin > tymax || tymin > tmax) return hitData;

    if(tymin > tmin) tmin = tymin;
    if(tymax < tmax) tmax = tymax;

    float tzmin = ((sign[2] ? max.z : min.z) - ray.origin.z) * invDir.z;
    float tzmax = ((sign[2] ? min.z : max.z) - ray.origin.z) * invDir.z;

    if(tmin > tzmax || tzmin > tmax) return hitData;

    if(tzmin > tmin) tmin = tzmin;
    if(tzmax < tmax) tmax = tzmax;

    if(tmax < 0) return hitData; // box is behind the ray

    float t = (tmin >= 0) ? tmin : tmax;

    hitData.hit = true;
    hitData.t = t;
    hitData.point = ray.parametrize(t);

    return hitData;
}

inline bool AABB::contains(const Vec3& p) const
{
    return (p.x >= min.x && p.x <= max.x) &&
           (p.y >= min.y && p.y <= max.y) &&
           (p.z >= min.z && p.z <= max.z);
}

inline void AABB::constructWireframe()
{
    const float thickness = (max.y - min.y) / 80; // TODO: Figure out maybe better scaling method
    const float offset = 0.0f; // TODO: Figure out where to do wireframe intersections for this to work
    const Color DEBUG(255, 0, 255);
    const Material mat = Material(
        DEBUG,
        DEBUG,
        Color(255, 255, 255),
        0.05f,
        0.55f,
        0.4f,
        100,
        false
    );

    // Edges for rectangles
    const Vec3 xShortEdge = Vec3(thickness, 0.0f, 0.0f);
    const Vec3 yShortEdge = Vec3(0.0f, thickness, 0.0f);
    const Vec3 zShortEdge = Vec3(0.0f, 0.0f, thickness);
    const Vec3 xLongEdge = Vec3(max.x - min.x + offset, 0.0f, 0.0f);
    const Vec3 yLongEdge = Vec3(0.0f, max.y - min.y + offset, 0.0f);
    const Vec3 zLongEdge = Vec3(0.0f, 0.0f, max.z - min.z + offset);

    // Normals for rectangles
    const Vec3 xNorm = Vec3(1.0f, 0.0f, 0.0f);
    const Vec3 yNorm = Vec3(0.0f, 1.0f, 0.0f);
    const Vec3 zNorm = Vec3(0.0f, 0.0f, 1.0f);

    // Corner to base rectangles off of
    const Vec3 tempMin = min - Vec3(offset);

    // Bottom Frame
    wireframe[0] = Rectangle(tempMin, -xNorm, yShortEdge, zLongEdge, mat);
    wireframe[1] = Rectangle(tempMin + zLongEdge, zNorm, yShortEdge, xLongEdge, mat);
    wireframe[2] = Rectangle(tempMin, -zNorm, yShortEdge, xLongEdge, mat);
    wireframe[3] = Rectangle(tempMin + xLongEdge, xNorm, yShortEdge, zLongEdge, mat);

    wireframe[4] = Rectangle(tempMin, -yNorm, xShortEdge, zLongEdge, mat);
    wireframe[5] = Rectangle(tempMin + zLongEdge, -yNorm, -zShortEdge, xLongEdge, mat);
    wireframe[6] = Rectangle(tempMin, -yNorm, zShortEdge, xLongEdge, mat);
    wireframe[7] = Rectangle(tempMin + xLongEdge, -yNorm, -xShortEdge, zLongEdge, mat);

    // Top Frame
    wireframe[8] = Rectangle(tempMin + yLongEdge, -xNorm, -yShortEdge, zLongEdge, mat);
    wireframe[9] = Rectangle(tempMin + zLongEdge + yLongEdge, zNorm, -yShortEdge, xLongEdge, mat);
    wireframe[10] = Rectangle(tempMin + yLongEdge, -zNorm, -yShortEdge, xLongEdge, mat);
    wireframe[11] = Rectangle(tempMin + xLongEdge + yLongEdge, xNorm, -yShortEdge, zLongEdge, mat);

    wireframe[12] = Rectangle(tempMin + yLongEdge, yNorm, xShortEdge, zLongEdge, mat);
    wireframe[13] = Rectangle(tempMin + zLongEdge + yLongEdge, yNorm, -zShortEdge, xLongEdge, mat);
    wireframe[14] = Rectangle(tempMin + yLongEdge, yNorm, zShortEdge, xLongEdge, mat);
    wireframe[15] = Rectangle(tempMin + xLongEdge + yLongEdge, yNorm, -xShortEdge, zLongEdge, mat);

    // Side Frames
    wireframe[16] = Rectangle(tempMin, -xNorm, zShortEdge, yLongEdge, mat);
    wireframe[17] = Rectangle(tempMin + zLongEdge, -xNorm, -zShortEdge, yLongEdge, mat);
    wireframe[18] = Rectangle(tempMin + xLongEdge, xNorm, zShortEdge, yLongEdge, mat);
    wireframe[19] = Rectangle(tempMin + xLongEdge + zLongEdge, xNorm, -zShortEdge, yLongEdge, mat);

    wireframe[20] = Rectangle(tempMin, -zNorm, xShortEdge, yLongEdge, mat);
    wireframe[21] = Rectangle(tempMin + zLongEdge, zNorm, xShortEdge, yLongEdge, mat);
    wireframe[22] = Rectangle(tempMin + xLongEdge, -zNorm, -xShortEdge, yLongEdge, mat);
    wireframe[23] = Rectangle(tempMin + xLongEdge + zLongEdge, zNorm, -xShortEdge, yLongEdge, mat);
}