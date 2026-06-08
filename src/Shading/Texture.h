#pragma once

#include <memory>

#include "Resources/TextureData.h"
#include "Shading/Color.h"

struct Texture {
    TextureData baseLevel;
    int maxLevel;

    enum class SampleFilter {
        Nearest,
        Linear,
        Nearest_Mipmap_Nearest,
        Linear_Mipmap_Nearest,
        Nearest_Mipmap_Linear,
        Linear_Mipmap_Linear
    };

    enum class WrapMode {
        Repeat,
        Mirrored_Repeat,
        Clamp_To_Edge
    };

    SampleFilter sampleFilter = SampleFilter::Linear;
    WrapMode wrapMode = WrapMode::Repeat;

    Texture() = default;

    Texture(const TextureData& textureData);

    Texture(const TextureData& textureData, const SampleFilter& sampleFilter);

    Texture(const TextureData& textureData, const SampleFilter& sampleFilter, const WrapMode& wrapMode);

    Color sample(const float& u, const float& v, const float& LOD) const;

    Color nearest(const float& u, const float& v, const int& mipmapLevel) const;

    Color linear(const float& u, const float& v, const int& mipmapLevel) const;

    void wrap(int& x, int& y, const int& width, const int& height) const;

    const TextureData* getMipmapData(const int& mipmapLevel) const;

    int maxMipmapLevel() const;

    bool exists() const;
};

#include "Texture.inl"