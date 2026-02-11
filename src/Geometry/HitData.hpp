#pragma once

#include "Shading/SurfaceProperties.h"
#include "Math/Math.h"

struct HitData {
    bool hit = false;
    float t;
    Vec3 point;
    Vec3 faceNormal;
    Vec3 shadingNormal;
    Material material;
};