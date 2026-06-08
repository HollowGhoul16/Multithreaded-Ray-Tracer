#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "TextureData.h"

struct TextureLoader {
    std::unordered_map<std::string, TextureData> textureDataCache;

    const std::string MISSING_TEXTURE_PATH = "../assets/textures/Missing_Texture_256x256.png";

    TextureLoader();

    TextureData loadTexture(const std::string& path, const bool fileExists);

    enum class FileType {
        PNG,
        JPG,
        INVALID
    };

    const std::unordered_map<std::string, FileType> extensionMap = {
        {".png", FileType::PNG},
        {".jpg", FileType::JPG},
        {".jpeg", FileType::JPG}
    };

    FileType getFileType(const std::string& path) const;
};