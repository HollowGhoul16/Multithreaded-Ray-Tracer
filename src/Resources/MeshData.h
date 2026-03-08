#pragma once

#include <memory>

#include "Geometry/Mesh.h"
#include "Math/Mat.h"

struct MeshData {
    std::shared_ptr<std::vector<Triangle>> triangles;

    MeshData() = default;

    MeshData(std::shared_ptr<std::vector<Triangle>>&& triangles);

    Mesh makeInstance(const Mat4& modelMatrix, const Material& mat) const;
};