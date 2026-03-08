#include "ResourceManager.h"

MeshData ResourceManager::loadObj(const std::string& path)
{
    if(getFileType(path) != FileType::Obj) {
        std::cerr << "File is not a .obj file with path (" << path << ")" << "\n";
        std::cerr << "Returning with empty MeshData" << "\n";
        return MeshData();
    }

    if(meshDataCache.find(path) != meshDataCache.end()) return meshDataCache[path];

    meshDataCache[path] = parseObj(path);

    return meshDataCache[path];
}

ResourceManager::FileType ResourceManager::getFileType(const std::string& path) const
{
    std::string extension = path.substr(path.find_last_of("."), path.size() - path.find_last_of("."));

    auto it = extensionMap.find(extension);
    if(it == extensionMap.end()) return FileType::Invalid;

    return it->second;
}