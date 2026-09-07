#pragma once

#include <functional>
#include <memory>
#include <string>

#include "../../include/stb_image.h"
#include "Shading/Color.h"

struct TextureData {
    int width, height;
    int channels;
    std::shared_ptr<unsigned char> data = nullptr;
    std::shared_ptr<TextureData> nextLevel = nullptr;

    enum class MipmapFilter {
        Point,
        Box,
        Triangle,
        BSpline,
        CatmullRom,
        MitchellNetravali,
        Lanczos,
        Gaussian
    };

    TextureData() = default;

    TextureData(const std::string& path);

    void generateMipmap(const MipmapFilter& filter);

    void filter(std::shared_ptr<TextureData>& newLevel, const std::function<float(float)>& kernel, float radius);

    void pointFilter(std::shared_ptr<TextureData>& newLevel); // Special case

    static float box(float x);

    static float triangle(float x);

    static float bspline(float x);

    static float catmullRom(float x);

    static float mitchellNetravali(float x);

    static float lanczos(float x);

    static float gaussian(float x);

    Color getPixel(const int x, const int y) const;

    void setPixel(const int x, const int y, Color color);

    int mipmapLevelCount() const;
};