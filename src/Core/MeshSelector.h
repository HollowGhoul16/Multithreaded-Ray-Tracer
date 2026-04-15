#pragma once

#include <unordered_set>
#include <vector>

#include "Geometry/Mesh.h"
#include "Math/Mat.h"

struct MeshSelector {
    std::unordered_set<Mesh*> selected;

    MeshSelector() = default;

    void applyTransform(const Mat4& transformation);

    std::vector<Mesh> duplicate();

    void select(Mesh* mesh);

    void deselect(Mesh* mesh);

    void deselectAll();
};

#include "MeshSelector.inl"