#include "Math.h"

namespace Math {
    inline float sinc(const float& x)
    {
        if(x <= EPSILON) return 1.0;
        return std::sin(PRECISE_PI * x) / (PRECISE_PI * x);
    }

    constexpr float degressToRadians(const float& theta)
    {
        return theta * (PI / 180.0f);
    }

    template<typename T>
    inline T clamp(const T& val, const T& min, const T& max)
    {
        return (val < min) ? min : (val > max) ? max : val;
    }
}