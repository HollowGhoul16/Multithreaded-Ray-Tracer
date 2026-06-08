#pragma once

#include "Vec.h"
#include "Mat.h"

#include "Math_Constants.hpp"

namespace Math {
    float sinc(float x);

    template<typename T>
    T clamp(const T& val, const T& min, const T& max);
}

#include "Math.inl"