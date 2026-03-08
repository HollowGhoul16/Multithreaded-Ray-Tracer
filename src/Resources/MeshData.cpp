#include "MeshData.h"

MeshData::MeshData(std::shared_ptr<std::vector<Triangle>>&& triangles)
                  : triangles(triangles) {}

Mesh MeshData::makeInstance(const Mat4& modelMatrix, const Material& mat) const
{
    std::vector<Triangle> newTriangles;
    std::vector<Triangle>& triangles = *(this->triangles);

    Vec3 triangleVerts[3];
    Vec2 textureCoords[3];
    Vec3 triangleNorms[3];

    for(const Triangle& triangle : triangles) {
        for(int i = 0; i < 3; ++i) {
            triangleVerts[i] = modelMatrix.matvec(triangle.vertices[i]);
            textureCoords[i] = triangle.texCoords[i];
            triangleNorms[i] = triangle.normals[i];
        }

        newTriangles.push_back(Triangle(triangleVerts, textureCoords, triangleNorms, mat));
    }

    return Mesh(std::move(newTriangles));
}