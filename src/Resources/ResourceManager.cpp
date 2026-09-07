#include "ResourceManager.h"

TextureData ResourceManager::loadTexture(const std::string& path)
{
    return textureLoader.loadTexture(path, fileExists(path));
}

Cubemap ResourceManager::loadCubemap(const std::array<std::string, 6>& paths)
{
    std::array<TextureData, 6> faces;

    for(int i = 0; i < 6; ++i) {
        const std::string& path = paths[i];
        faces[i] = textureLoader.loadTexture(path, fileExists(path));
        // faces[i].generateMipmap(TextureData::MipmapFilter::Triangle);
    }

    return Cubemap(faces);
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