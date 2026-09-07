#pragma once

#include "Texture.h"

struct Cubemap {
    Texture xPos, xNeg, yPos, yNeg, zPos, zNeg; // zPos is the back face

    Cubemap(const std::array<TextureData, 6>& faces);

    Color sample(const Vec3& direction) const;

    void toTextureSpace(float& u, float& v, const float& w) const;

    float flipTexCoord(const float& x) const;
};

#include "Cubemap.inl"