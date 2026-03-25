#pragma once

#include <functional>
#include <unordered_map>
#include <chrono>
#include <fstream>

#include "Utils.h"

namespace Screenshot
{
    enum class ScreenshotFileFormat {
        BMP,
        PPM
    };

    extern ScreenshotFileFormat currentSSFileFormat; // TODO: Add way for user to change format

    using ScreenshotWriter = std::function<void(const std::string&, const unsigned char[], const int, const int)>;

    extern const std::unordered_map<ScreenshotFileFormat, ScreenshotWriter> screenshotWriteFunctionMap;

    void screenshot(const unsigned char imageData[], const int imageWidth, const int imageHeight); // Uses currentSSFileFormat

    void writeBMP(const std::string& filePath, const unsigned char imageData[], int imageWidth, int imageHeight);

    void writePPM(const std::string& filePath, const unsigned char imageData[], int imageWidth, int imageHeight);
};