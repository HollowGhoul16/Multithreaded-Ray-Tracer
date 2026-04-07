#include "MeshSelector.h"

inline void MeshSelector::applyTransform(const Mat4& transformation)
{
    for(Mesh* mesh : selected) mesh->applyTransform(transformation);
}

inline void MeshSelector::select(Mesh* mesh)
{
    mesh->markSelected();
    selected.insert(mesh);
}

inline void MeshSelector::deselect(Mesh* mesh)
{
    mesh->markDeselected();
    selected.erase(mesh);
}

inline void MeshSelector::deselectAll()
{
    for(Mesh* mesh : selected) mesh->markDeselected();
    selected.clear();
}