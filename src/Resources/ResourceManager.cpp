#include "ResourceManager.h"

TextureData ResourceManager::loadTexture(const std::string& path)
{
    return textureLoader.loadTexture(path, fileExists(path));
}

MeshData ResourceManager::loadMesh(const std::string& path)
{
    return meshLoader.loadMesh(path, fileExists(path));
}

bool ResourceManager::fileExists(const std::string& path) const
{
    struct stat buf;
    return (stat(path.c_str(), &buf) == 0);
}