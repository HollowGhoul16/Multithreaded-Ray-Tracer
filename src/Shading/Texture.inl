#include "Texture.h"

inline Texture::Texture(const TextureData& textureData)
                       : baseLevel(textureData)
{
    this->maxLevel = maxMipmapLevel();
}

inline Texture::Texture(const TextureData& textureData, const SampleFilter& sampleFilter)
                       : baseLevel(textureData), sampleFilter(sampleFilter)
{
    this->maxLevel = maxMipmapLevel();
}

inline Texture::Texture(const TextureData& textureData, const SampleFilter& sampleFilter, const WrapMode& wrapMode)
                       : baseLevel(textureData), sampleFilter(sampleFilter), wrapMode(wrapMode)
{
    this->maxLevel = maxMipmapLevel();
}

inline Color Texture::sample(const float& u, const float& v, const float& LOD) const
{
    const int nearestLevel = static_cast<int>(std::ceil(LOD + 0.5f) - 1); // From Khronos documentation
    const int minLevel     = static_cast<int>(std::floor(LOD));
    const int maxLevel     = static_cast<int>(std::ceil(LOD));

    // if(!exists()) return Color();

    switch(sampleFilter) {
        case SampleFilter::Nearest:
            return nearest(u, v, 0);
            break;
        case SampleFilter::Linear:
            return linear(u, v, 0);
            break;
        case SampleFilter::Nearest_Mipmap_Nearest:
            return nearest(u, v, nearestLevel);
            break;
        case SampleFilter::Linear_Mipmap_Nearest:
            return linear(u, v, nearestLevel);
            break;
        case SampleFilter::Nearest_Mipmap_Linear:
            return Color::lerp(nearest(u, v, minLevel), nearest(u, v, maxLevel), (LOD - std::floor(LOD)));
            break;
        case SampleFilter::Linear_Mipmap_Linear:
            return Color::lerp(linear(u, v, minLevel), linear(u, v, maxLevel), (LOD - std::floor(LOD)));
            break;
        default:
            return Color();
    }
}

inline Color Texture::nearest(const float& u, const float& v, const int& mipmapLevel) const
{
    const TextureData* data = getMipmapData(mipmapLevel);
    int x = static_cast<int>(u * data->width);
    int y = static_cast<int>(v * data->height);
    wrap(x, y, data->width, data->height);

    return data->getPixel(x, y);
}

inline Color Texture::linear(const float& u, const float& v, const int& mipmapLevel) const
{
    const TextureData* data = getMipmapData(mipmapLevel);

    int x0 = static_cast<int>(std::floor((u * data->width)  - 0.5f));
    int y0 = static_cast<int>(std::floor((v * data->height) - 0.5f));

    float fracX = ((u * data->width )  - 0.5f) - x0;
    float fracY = ((v * data->height)  - 0.5f) - y0;

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    wrap(x0, y0, data->width, data->height);
    wrap(x1, y1, data->width, data->height);

    Color topLeft     = data->getPixel(x0, y0);
    Color topRight    = data->getPixel(x1, y0);
    Color bottomLeft  = data->getPixel(x0, y1);
    Color bottomRight = data->getPixel(x1, y1);

    Color top = Color::lerp(topLeft, topRight, fracX);
    Color bottom = Color::lerp(bottomLeft, bottomRight, fracX);

    return Color::lerp(top, bottom, fracY);
}

inline void Texture::wrap(int& x, int& y, const int& width, const int& height) const
{
    switch(wrapMode) {
        case WrapMode::Repeat:
            if(x == -1) x = width - 1;
            if(y == -1) y = height - 1;
            x = x % width;
            y = y % height;
            break;
        case WrapMode::Mirrored_Repeat: // TODO: Implement mirrored repeat, make sure works with linear sampling
            // if(x == -1 || x == width - 1) {
            //     x = width - 1;
            //     x = 0;
            // }
            // if(y == -1) y = height - 1;
            // if(x1 == data->width) x1 = 0;
            // if(y == height) y = 0;
            break;
        case WrapMode::Clamp_To_Edge:
            x = Math::clamp(x, 0, width - 1);
            y = Math::clamp(y, 0, height - 1);
            break;
    }
}

inline const TextureData* Texture::getMipmapData(const int& mipmapLevel) const
{
    const TextureData* sampleLevel = &baseLevel;

    for(int i = 0; i < std::min(mipmapLevel, maxLevel); ++i) sampleLevel = sampleLevel->nextLevel.get();

    return sampleLevel;
}

inline int Texture::maxMipmapLevel() const
{
    int maxLevel = 0;
    const TextureData* temp = &baseLevel;

    while(temp->nextLevel.get() != nullptr) {
        temp = temp->nextLevel.get();
        maxLevel += 1;
    }

    return maxLevel;
}

inline bool Texture::exists() const
{
    return baseLevel.data.get() != nullptr;
}