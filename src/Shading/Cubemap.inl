#include "Cubemap.h"

inline Cubemap::Cubemap(const std::array<TextureData, 6>& faces)
{
    xPos = Texture(faces[0], Texture::SampleFilter::Nearest, Texture::WrapMode::Clamp_To_Edge);
    xNeg = Texture(faces[1], Texture::SampleFilter::Nearest, Texture::WrapMode::Clamp_To_Edge);
    yPos = Texture(faces[2], Texture::SampleFilter::Nearest, Texture::WrapMode::Clamp_To_Edge);
    yNeg = Texture(faces[3], Texture::SampleFilter::Nearest, Texture::WrapMode::Clamp_To_Edge);
    zPos = Texture(faces[4], Texture::SampleFilter::Nearest, Texture::WrapMode::Clamp_To_Edge);
    zNeg = Texture(faces[5], Texture::SampleFilter::Nearest, Texture::WrapMode::Clamp_To_Edge);
}

inline Color Cubemap::sample(const Vec3& direction) const
{
    float x = direction.x;
    float y = direction.y;
    float z = direction.z;

    // Longest value is the face hit
    if((fabs(x) > fabs(y)) && (fabs(x) > fabs(z))) {
        toTextureSpace(z, y, x);

        if(x > 0.0f) return xPos.sample(z, y, 0);
        else         return xNeg.sample(z, flipTexCoord(y), 0);
    }
    else if(fabs(y) > fabs(z)) {
        toTextureSpace(x, z, y);

        if(y > 0.0f) return yPos.sample(x, z, 0);
        else         return yNeg.sample(flipTexCoord(x), z, 0);
    }
    else {
        toTextureSpace(x, y, z);

        if(z > 0.0f) return zPos.sample(flipTexCoord(x), y, 0);
        else         return zNeg.sample(flipTexCoord(x), flipTexCoord(y), 0);
    }
}

inline void Cubemap::toTextureSpace(float& u, float& v, const float& w) const
{
    // Normalize to [-1.0, 1.0] range
    u /= w;
    v /= w;

    // To texture space [0.0, 1.0] range
    u = (u + 1.0f) / 2.0f;
    v = (v + 1.0f) / 2.0f;
}

inline float Cubemap::flipTexCoord(const float& x) const
{
    return (x - 1.0f) * -1;
}