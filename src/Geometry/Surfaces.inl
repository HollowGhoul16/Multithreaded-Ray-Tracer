#include "Surfaces.h"

// Surface struct

inline Surface::Surface(const Material& m, const SurfaceType& t)
                       : material(m), type(t) {};

// Sphere struct

inline Sphere::Sphere() : Surface(Material(), SurfaceType::Sphere), radius(1.0f) {}

inline Sphere::Sphere(const Vec3& cent, const float& r, const Material& m)
                     : Surface(m, SurfaceType::Sphere), center(cent), radius(r) {};

inline HitData Sphere::intersection(const Ray& ray) const 
{
    HitData hitData;

    Vec3 CtoO = center - ray.origin;
    float a = ray.direction.dot(ray.direction);
    float h = CtoO.dot(ray.direction); // Using b = -2h (from raytracing in one weekend)
    float c = CtoO.dot(CtoO) - radius * radius;

    float discriminant = (h * h) - (a * c);

    if(discriminant < 0) return hitData;

    float t = (h - std::sqrt(discriminant)) / a;

    // Makes sure to grab the closest point to the camera
    if(t > 0.001) {
        hitData.hit = true;
        hitData.t = t;
        hitData.point = ray.parametrize(t);
        hitData.faceNormal = normal(ray, hitData.point);
        hitData.shadingNormal = hitData.faceNormal;
        hitData.material = material;
        hitData.surfaceType = type;
        hitData.surfacePtr = this;
        return hitData;
    }

    t = (h + std::sqrt(discriminant)) / a;

    if(t > 0.001) {
        hitData.hit = true;
        hitData.t = t;
        hitData.point = ray.parametrize(t);
        hitData.faceNormal = normal(ray, hitData.point);
        hitData.shadingNormal = hitData.faceNormal;
        hitData.material = material;
        hitData.surfaceType = type;
        hitData.surfacePtr = this;
    }

    return hitData;
}

inline Vec3 Sphere::normal(const Ray& ray, const Vec3& point) const 
{
    Vec3 normalVec = (point - center).normalize();
    if(normalVec.dot(ray.direction) < 0) return normalVec;
    else return -normalVec;
}

inline std::pair<Vec3, Vec3> Sphere::getBounds() const
{
    Vec3 extent(radius, radius, radius);
    Vec3 min = center - extent;
    Vec3 max = center + extent;

    return {min, max};
}

// Plane struct

inline Plane::Plane(const Vec3& p, const Vec3& n, const Material& m)
                   : Surface(m, SurfaceType::Plane), point(p), normalVec(n) {};

inline HitData Plane::intersection(const Ray& ray) const 
{
    HitData hitData;

    float denom = ray.direction.dot(normalVec);

    if(fabs(denom) < Math::EPSILON) return hitData; // Prevent t = inf

    float t = (point - ray.origin).dot(normalVec) / denom;

    // Makes sure to grab the point IF in front of camera
    if(t > 0.001f) {
        hitData.hit = true;
        hitData.t = t;
        hitData.point = ray.parametrize(t);
        hitData.faceNormal = normal(ray, hitData.point);
        hitData.shadingNormal = hitData.faceNormal;
        hitData.material = material;
        hitData.surfaceType = type;
        hitData.surfacePtr = this;
    }

    return hitData;
}

inline Vec3 Plane::normal(const Ray& ray, const Vec3& point) const 
{
    if(normalVec.dot(ray.direction) < 0) return normalVec;
    else return -normalVec;
}

// Rectangle struct

inline Rectangle::Rectangle()
    : Plane(Vec3(), Vec3(), Material()), 
      corner(Vec3()), 
      edge1(Vec3()), 
      edge2(Vec3()) 
{
    type = SurfaceType::Rectangle;
}

inline Rectangle::Rectangle(
    const Vec3& c,
    const Vec3& n,
    const Vec3& e1,
    const Vec3& e2,
    const Material& m)
    : Plane(c, n, m),
      corner(c),
      edge1(e1),
      edge2(e2)
{
    type = SurfaceType::Rectangle;
}

// TODO: Figure out ray hitting rectangle edge, edge case (literally)
inline HitData Rectangle::intersection(const Ray &ray) const
{
    HitData hitData = Plane::intersection(ray);
    if(!hitData.hit) return hitData; // Check if on rectangle's plane

    Vec3 hitPoint = hitData.point - corner;

    float edge1Mag = edge1.magnitude();
    float proj1 = hitPoint.dot(edge1) / edge1Mag;

    if(proj1 > edge1Mag || proj1 < 0) {
        hitData.hit = false;
        return hitData;
    }

    float edge2Mag = edge2.magnitude();
    float proj2 = hitPoint.dot(edge2) / edge2Mag;

    if(proj2 > edge2Mag || proj2 < 0) hitData.hit = false;

    hitData.surfacePtr = this;

    return hitData;
}

inline Vec3 Rectangle::normal(const Ray &ray, const Vec3 &point) const
{
    return Plane::normal(ray, point);
}

inline std::pair<Vec3, Vec3> Rectangle::getBounds() const
{
    Vec3 oppositeCorner = corner + edge1 + edge2;
    Vec3 min, max;

    if(corner.x < oppositeCorner.x ||
       corner.y < oppositeCorner.y ||
       corner.z < oppositeCorner.z)
    {
        min = corner;
        max = oppositeCorner;
    }
    else {
        min = oppositeCorner;
        max = corner;
    }

    return {min, max};
}

// Triangle Struct

inline Triangle::Triangle(const Vec3 v[3], const Vec2 tc[3], const Vec3 n[3], const Material &m)
                         : Plane(v[0], (v[1] - v[0]).cross(v[2] - v[1]), m)
{
    for(int i = 0; i < 3; ++i) {
        vertices[i] = v[i];
        texCoords[i] = tc[i];
        normals[i] = n[i].normalize();
        if(normalVec.dot(normals[i]) < 0) normals[i] = -normals[i]; // Correct the vertex normals from .obj using geometric normal
    }

    for(int i = 0; i < 3; ++i) edges[i] = vertices[(i + 1) % 3] - vertices[i];

    type = SurfaceType::Triangle;
}

inline HitData Triangle::intersection(const Ray& ray) const
{
    HitData hitData;

    Vec3 v0v1 = edges[0];
    Vec3 v0v2 = -edges[2];
    Vec3 pvec = ray.direction.cross(v0v2);
    float det = v0v1.dot(pvec);

    if(fabs(det) < Math::EPSILON) return hitData;

    float invDet = 1 / det;

    Vec3 tvec = ray.origin - vertices[0];
    float u = tvec.dot(pvec) * invDet;
    if(u < 0 || u > 1) return hitData;

    Vec3 qvec = tvec.cross(v0v1);
    float v = ray.direction.dot(qvec) * invDet;
    if(v < 0 || u + v > 1) return hitData;
    
    float t = v0v2.dot(qvec) * invDet;
    if(t < 0) return hitData;

    hitData.hit = true;
    hitData.t = t;
    hitData.texCoord = (texCoords[0] * (1.0f - u - v) + texCoords[1] * u + texCoords[2] * v);
    hitData.point = ray.parametrize(t);
    hitData.faceNormal = normal(ray, hitData.point);
    hitData.shadingNormal = (normals[0] * (1.0f - u - v) + normals[1] * u + normals[2] * v).normalize();
    hitData.material = material;
    hitData.surfaceType = type;
    hitData.surfacePtr = this;

    return hitData;
}

inline Vec3 Triangle::normal(const Ray& ray, const Vec3& point) const
{
    return Plane::normal(ray, point);
}

inline std::pair<Vec3, Vec3> Triangle::getBounds() const
{
    Vec3 min, max;

    for(int i = 0; i < 3; ++i) {
        min[i] = std::min(std::min(vertices[0][i], vertices[1][i]), vertices[2][i]);
        max[i] = std::max(std::max(vertices[0][i], vertices[1][i]), vertices[2][i]);
    }

    return {min, max};
}

inline float Triangle::getLODConstant(const Mat3& modelMatrix) const
{
    float triangleAreaDoubled = (modelMatrix.matvec(edges[0]).cross(modelMatrix.matvec(edges[2]))).magnitude();
    float texCoordsAreaDoubled = ((texCoords[1] - texCoords[0]).cross(texCoords[2] - texCoords[1]));

    return 0.5 * log2(std::fabs(texCoordsAreaDoubled) / triangleAreaDoubled);
}