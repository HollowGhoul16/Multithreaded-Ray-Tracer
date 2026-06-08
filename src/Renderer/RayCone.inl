#include "RayCone.h"

inline void RayCone::updateWidth(const float& t)
{
    width += spreadAngle * t;
}

inline void RayCone::updateSpreadAngle(const float& surfaceSpreadAngle)
{
    spreadAngle += surfaceSpreadAngle;
}