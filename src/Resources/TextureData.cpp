#include "TextureData.h"

TextureData::TextureData(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    data.reset(std::move(stbi_load(path.data(), &width, &height, &channels, STBI_rgb)));
    channels = STBI_rgb;
}

void TextureData::generateMipmap(const MipmapFilter& filter)
{
    TextureData* currentLevel = this;

    for(int i = 0; i < mipmapLevelCount(); ++i) {
        const int NEW_DATA_SIZE = (currentLevel->width / 2) * (currentLevel->height / 2) * channels;

        std::shared_ptr<TextureData> newLevel = std::make_shared<TextureData>();
        newLevel->width    = currentLevel->width  / 2;
        newLevel->height   = currentLevel->height / 2;
        newLevel->channels = currentLevel->channels;

        std::shared_ptr<unsigned char> newData(new unsigned char[NEW_DATA_SIZE], std::default_delete<unsigned char[]>());
        newLevel->data = newData;

        switch(filter) {
                   case MipmapFilter::Point:             currentLevel->pointFilter(newLevel);
            break; case MipmapFilter::Box:               currentLevel->filter(newLevel, box, 0.5f);
            break; case MipmapFilter::Triangle:          currentLevel->filter(newLevel, triangle, 1.0f);
            break; case MipmapFilter::BSpline:           currentLevel->filter(newLevel, bspline, 2.0f);
            break; case MipmapFilter::CatmullRom:        currentLevel->filter(newLevel, catmullRom, 2.0f);
            break; case MipmapFilter::MitchellNetravali: currentLevel->filter(newLevel, mitchellNetravali, 2.0f);
            break; case MipmapFilter::Lanczos:           currentLevel->filter(newLevel, lanczos, 2.0f);
            break; case MipmapFilter::Gaussian:          currentLevel->filter(newLevel, gaussian, 2.0f);
        }

        currentLevel->nextLevel = newLevel;
        currentLevel = currentLevel->nextLevel.get();
    }
}

void TextureData::filter(std::shared_ptr<TextureData>& newLevel, const std::function<float(float)>& kernel, float radius)
{
    const float scaleX = static_cast<float>(this->width)  / newLevel->width;
    const float scaleY = static_cast<float>(this->height) / newLevel->height;

    for(int j = 0; j < newLevel->height; ++j) {
        for(int i = 0; i < newLevel->width; ++i) {
            float centerX = (i + 0.5f) * scaleX;
            float centerY = (j + 0.5f) * scaleY;

            Color newColor;
            float totalWeight = 0.0f;

            int minX = static_cast<int>(ceil (centerX - radius - 0.5f));
            int maxX = static_cast<int>(floor(centerX + radius - 0.5f));

            int minY = static_cast<int>(ceil (centerY - radius - 0.5f));
            int maxY = static_cast<int>(floor(centerY + radius - 0.5f));

            for(int sampleY = minY; sampleY <= maxY; ++sampleY) {
                for(int sampleX = minX; sampleX <= maxX; ++sampleX) {
                    int indexX = std::min(std::max(sampleX, 0), this->width - 1);
                    int indexY = std::min(std::max(sampleY, 0), this->height - 1);

                    float dx = centerX - (sampleX + 0.5f);
                    float dy = centerY - (sampleY + 0.5f);

                    float wx = kernel(dx / scaleX);
                    float wy = kernel(dy / scaleY);
                    float weight = wx * wy;

                    Color sample = this->getPixel(indexX, indexY);

                    newColor = newColor + (sample * weight);
                    totalWeight += weight;
                }
            }

            newColor = newColor / totalWeight;
            newLevel->setPixel(i, j, newColor);
        }
    }
}

void TextureData::pointFilter(std::shared_ptr<TextureData>& newLevel)
{
    const float scaleX = static_cast<float>(this->width)  / newLevel->width;
    const float scaleY = static_cast<float>(this->height) / newLevel->height;

    for(int j = 0; j < newLevel->height; ++j) {
        for(int i = 0; i < newLevel->width; ++i) {
            float centerX = (i + 0.5f) * scaleX;
            float centerY = (j + 0.5f) * scaleY;

            int sampleX = static_cast<int>(centerX);
            int sampleY = static_cast<int>(centerY);

            sampleX = std::min(std::max(sampleX, 0), this->width - 1);
            sampleY = std::min(std::max(sampleY, 0), this->height - 1);

            Color newColor = this->getPixel(sampleX, sampleY);

            newLevel->setPixel(i, j, newColor);
        }
    }
}

float TextureData::box(float x)
{
    return (fabs(x) <= 0.5f) ? 1.0f : 0.0f;
}

float TextureData::triangle(float x)
{
    return (fabs(x) < 1.0f) ? (1.0f - x) : 0.0f;
}

float TextureData::bspline(float x)
{
    x = fabs(x);
    float ret = 0.0f;

    if(x <= 1.0f) {
        ret = -3 * std::pow((1 - x), 3) +
               3 * std::pow((1 - x), 2) +
               3 * (1 - x) +
               1;
    }
    else if(x <= 2.0f) {
        ret = std::pow((2 - x), 3);
    }

    return ret / 6.0f;
}

float TextureData::catmullRom(float x)
{
    x = fabs(x);
    float ret = 0.0f;

    if(x <= 1.0f) {
        ret = -3 * std::pow((1 - x), 3) +
               4 * std::pow((1 - x), 2) +
               (1 - x);
    }
    else if(x <= 2.0f) {
        ret = std::pow((2 - x), 3) -
              std::pow((2 - x), 2);
    }

    return ret / 2.0f;
}

float TextureData::mitchellNetravali(float x)
{
    x = fabs(x);
    float ret = 0.0f;

    if(x <= 1.0f) {
        ret = -15 * std::pow((1 - x), 3) +
               18 * std::pow((1 - x), 2) +
               9 * (1 - x) +
               2;
    }
    else if(x <= 2.0f) {
        ret = 5 * std::pow((2 - x), 3) -
              3 * std::pow((2 - x), 2);
    }

    return ret / 18.0f;
}

float TextureData::lanczos(float x)
{
    x = fabs(x);

    return (x < 2.0f) ? Math::sinc(x) * Math::sinc(x / 2.0f) : 0;
}

float TextureData::gaussian(float x)
{
    const int SIGMA = 2;
    return (1 / (std::sqrt(2 * Math::PRECISE_PI) * SIGMA)) * (std::pow(Math::PRECISE_E, -((x * x) / (2 * SIGMA * SIGMA))));
}

Color TextureData::getPixel(const int x, const int y) const
{
    const int index = (x + (y * width)) * channels;

    Color pixel;
    pixel.r = data.get()[index];
    pixel.g = data.get()[index + 1];
    pixel.b = data.get()[index + 2];
    pixel.toLinear();

    return pixel;
}

void TextureData::setPixel(const int x, const int y, Color color)
{
    const int index = (x + (y * width)) * channels;

    color.toSRGB();
    data.get()[index] = color.r;
    data.get()[index + 1] = color.g;
    data.get()[index + 2] = color.b;
}

int TextureData::mipmapLevelCount() const
{
    return std::ceil(std::log2(std::max(width, height)));
}