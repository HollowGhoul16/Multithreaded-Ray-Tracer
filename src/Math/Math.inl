#include "Math.h"

namespace Math {
    inline float sinc(float x)
    {
        if(x <= EPSILON) return 1.0;
        return std::sin(PRECISE_PI * x) / (PRECISE_PI * x);
    }

    template<typename T>
    inline T clamp(const T& val, const T& min, const T& max)
    {
        return (val < min) ? min : (max > val) ? max : val;
    }
}