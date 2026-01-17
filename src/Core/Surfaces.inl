#include "Surfaces.h"

// Surface struct

constexpr Surface::Surface(const Material& m)
                          : material(m) {};

// Sphere struct

constexpr Sphere::Sphere(const Vec3& cent, const float& r, const Material& m)
                        : Surface(m), center(cent), radius(r) {};

inline std::pair<bool, float> Sphere::intersection(const Ray& ray) const 
{
    Vec3 CtoO = center - ray.origin;
    float a = ray.direction.dot(ray.direction);
    float h = CtoO.dot(ray.direction); // Using b = -2h (from raytracing in one weekend)
    float c = CtoO.dot(CtoO) - radius * radius;

    float discriminant = (h * h) - (a * c);

    if (discriminant < 0) return {false, 0.0};

    float t = (h - std::sqrt(discriminant)) / a;

    // Makes sure to grab the closest point to the camera
    if(t > 0.001) return {true, t};

    t = (h + std::sqrt(discriminant)) / a;
    if (t > 0.001) return {true, t};
    else return {false, 0.0};
}

inline Vec3 Sphere::normal(const Ray& ray, const Vec3& point) const 
{
    Vec3 normalVec = (point - center).normalize();
    if(normalVec.dot(ray.direction) < 0) return normalVec;
    else return -normalVec;
}

// Plane struct

constexpr Plane::Plane(const Vec3& p, const Vec3& n, const Material& m)
                      : Surface(m), point(p), normalVec(n) {};

inline std::pair<bool, float> Plane::intersection(const Ray& ray) const 
{
    float denom = ray.direction.dot(normalVec);

    if (fabs(denom) < 1e-6f) return {false, 0.0f}; // Prevent t = inf

    float t = (point - ray.origin).dot(normalVec) / denom;

    // Makes sure to grab the point IF in front of camera
    if(t > 0.001f) return {true, t};
    else return {false, 0.0};
}

inline Vec3 Plane::normal(const Ray& ray, const Vec3& point) const 
{
    if(normalVec.dot(ray.direction) < 0) return normalVec;
    else return -normalVec;
}

// Rectangle struct

constexpr Rectangle::Rectangle(
    const Vec3& c,
    const Vec3& n,
    const Vec3& e1,
    const Vec3& e2,
    const Material& m)
    : Plane(c, n, m),
      corner(c),
      edge1(e1),
      edge2(e2)
{}

inline std::pair<bool, float> Rectangle::intersection(const Ray &ray) const
{
    std::pair<bool, float> intersection = Plane::intersection(ray);
    if(intersection.first == false) return intersection; // Check if on rectangle's plane

    Vec3 vec = ray.parametrize(intersection.second) - corner;

    float edge1Mag = edge1.magnitude();
    float proj1 = vec.dot(edge1) / edge1Mag;
    if(proj1 > edge1Mag || proj1 < 0) return {false, 0.0};

    float edge2Mag = edge2.magnitude();
    float proj2 = vec.dot(edge2) / edge2Mag;
    if(proj2 > edge2Mag || proj2 < 0) return {false, 0.0};

    return {true, intersection.second};
}

inline Vec3 Rectangle::normal(const Ray &ray, const Vec3 &point) const
{
    return Plane::normal(ray, point);
}