#pragma once

#include <sys/stat.h>

#include "TextureLoader.h"
#include "MeshLoader.h"

struct ResourceManager {
    TextureLoader textureLoader;
    MeshLoader meshLoader;

    TextureData loadTexture(const std::string& path);

    MeshData loadMesh(const std::string& path);

    bool fileExists(const std::string& path) const;
};