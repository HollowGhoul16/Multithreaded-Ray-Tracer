#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "Parsers/ObjParser.h"
#include "MeshData.h"

struct ResourceManager {
    std::unordered_map<std::string, MeshData> meshDataCache;

    MeshData loadObj(const std::string& path);

    enum class FileType {
        Obj,
        Invalid
    };

    const std::unordered_map<std::string, FileType> extensionMap = {
        {".obj", FileType::Obj}
    };

    FileType getFileType(const std::string& path) const;
};