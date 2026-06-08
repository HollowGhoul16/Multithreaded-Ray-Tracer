#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
    if(getFileType(MISSING_TEXTURE_PATH) == FileType::INVALID) {
        std::cerr << "Something went wrong with loading missing texture" << "\n";
        textureDataCache[MISSING_TEXTURE_PATH] = TextureData();
    }
    else {
        textureDataCache[MISSING_TEXTURE_PATH] = TextureData(MISSING_TEXTURE_PATH);
    }
}

TextureData TextureLoader::loadTexture(const std::string& path, const bool fileExists)
{
    if(!fileExists) {
        std::cerr << "Texture does not exist with path (" << path << ")" << "\n";
        std::cerr << "Returning with missing texture" << "\n";
        return textureDataCache[MISSING_TEXTURE_PATH];
    }

    if(getFileType(path) == FileType::INVALID) {
        std::cerr << "Texture is not a supported filetype with path (" << path << ")" << "\n";
        std::cerr << "Returning with missing texture" << "\n";
        return textureDataCache[MISSING_TEXTURE_PATH];
    }

    if(textureDataCache.find(path) != textureDataCache.end()) return textureDataCache[path];

    textureDataCache[path] = TextureData(path);

    return textureDataCache[path];
}

TextureLoader::FileType TextureLoader::getFileType(const std::string& path) const
{
    std::string extension = path.substr(path.find_last_of("."), path.size() - path.find_last_of("."));

    auto it = extensionMap.find(extension);
    if(it == extensionMap.end()) return FileType::INVALID;

    return it->second;
}