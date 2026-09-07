#pragma once

#include <sys/stat.h>

#include "../Shading/Cubemap.h"
#include "TextureLoader.h"
#include "MeshLoader.h"

struct ResourceManager {
    TextureLoader textureLoader;
    MeshLoader meshLoader;

    TextureData loadTexture(const std::string& path);

    Cubemap loadCubemap(const std::array<std::string, 6>& paths);

    MeshData loadMesh(const std::string& path);

    bool fileExists(const std::string& path) const;
};