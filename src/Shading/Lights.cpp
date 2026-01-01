#include "Lights.h"

// DirectionalLight struct

DirectionalLight::DirectionalLight(const Vec3& d, const float& i)
                                  : direction(d.normalize()), intensity(i) {};