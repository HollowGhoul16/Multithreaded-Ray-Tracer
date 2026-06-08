#include "MeshLoader.h"

MeshData MeshLoader::loadMesh(const std::string& path, const bool fileExists)
{
    if(!fileExists) {
        std::cerr << "Mesh file does not exist with path (" << path << ")" << "\n";
        std::cerr << "Returning with empty MeshData" << "\n";
        return MeshData();
    }

    FileType fileType = getFileType(path);

    if(meshDataCache.find(path) != meshDataCache.end()) return meshDataCache[path];

    switch(fileType)
    {
        case FileType::OBJ: meshDataCache[path] = parseObj(path);
        break; case FileType::INVALID:
            std::cerr << "Mesh is not a supported filetype with path (" << path << ")" << "\n";
            std::cerr << "Returning with empty MeshData" << "\n";
            return MeshData();
    }

    return meshDataCache[path];
}

MeshLoader::FileType MeshLoader::getFileType(const std::string& path) const
{
    std::string extension = path.substr(path.find_last_of("."), path.size() - path.find_last_of("."));

    auto it = extensionMap.find(extension);
    if(it == extensionMap.end()) return FileType::INVALID;

    return it->second;
}