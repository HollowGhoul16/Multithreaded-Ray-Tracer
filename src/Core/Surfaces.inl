#include "Surfaces.h"

// Surface struct

constexpr Surface::Surface(const Material& m)
                          : material(m) {};

// Sphere struct

constexpr Sphere::Sphere(const Vec3& cent, const float& r, const Material& m)
                        : Surface(m), center(cent), radius(r) {};

inline std::pair<bool, float> Sphere::intersection(const Ray& ray) const 
{
    Vec3 CtoO = ray.origin - this->center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0 * CtoO.dot(ray.direction);
    float c = CtoO.dot(CtoO) - this->radius * this->radius;

    float discriminant = b * b - (4 * a * c);

    if (discriminant < 0) return {false, 0.0};

    float t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    float t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

    // Makes sure to grab the closest point to the camera
    if(t1 > 0.001) return {true, t1};
    else if (t2 > 0.001) return {true, t2};
    else return {false, 0.0};
}

inline Vec3 Sphere::normal(const Vec3& point) const 
{
    return (point - center).normalize();
}

// Plane struct

constexpr Plane::Plane(const Vec3& p, const Vec3& n, const Material& m)
                      : Surface(m), point(p), normalVec(n) {};

inline std::pair<bool, float> Plane::intersection(const Ray& ray) const 
{
    float t = ((point - ray.origin).dot(normalVec)) / (ray.direction.dot(normalVec));

    // Makes sure to grab the point IF in front of camera
    if(t > 0) return {true, t};
    else return {false, 0.0};
}

inline Vec3 Plane::normal(const Vec3& point) const 
{
    return normalVec;
}