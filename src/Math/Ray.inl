#include "Ray.h"

inline Ray::Ray(const Vec3& o, const Vec3& d)
               : origin(o), direction(d) {};

inline Vec3 Ray::parametrize(const float& t) const 
{
    return origin + direction * t;
}