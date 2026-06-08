#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "Parsers/ObjParser.h"
#include "MeshData.h"

struct MeshLoader {
    std::unordered_map<std::string, MeshData> meshDataCache;

    MeshData loadMesh(const std::string& path, const bool fileExists);

    enum class FileType {
        OBJ,
        INVALID
    };

    const std::unordered_map<std::string, FileType> extensionMap = {
        {".obj", FileType::OBJ}
    };

    FileType getFileType(const std::string& path) const;
};