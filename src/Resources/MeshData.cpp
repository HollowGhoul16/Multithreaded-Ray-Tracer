#include "MeshData.h"

MeshData::MeshData(std::shared_ptr<std::vector<Triangle>>&& triangles)
                  : triangles(triangles) {}

Mesh MeshData::makeInstance(const Mat4& modelMatrix, const Material& mat) const
{
    Mesh mesh(triangles, modelMatrix);
    mesh.material = mat;

    return mesh;
}