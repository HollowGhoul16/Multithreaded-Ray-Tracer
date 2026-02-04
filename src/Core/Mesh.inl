#include "Mesh.h"

inline Mesh::Mesh(std::vector<Surface*>&& surfaces)
                 : surfaces(surfaces)
{
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(std::numeric_limits<float>::lowest());

    for(const Surface* surface : surfaces) {
        std::pair<Vec3, Vec3> minMaxBounds = surface->getBounds();

        for(int i = 0; i < 3; ++i) {
            min[i] = std::min(min[i], minMaxBounds.first[i]);
            max[i] = std::max(max[i], minMaxBounds.second[i]);
        }
    }

    aabb.min = min;
    aabb.max = max;
}

inline Mesh::Mesh(Mesh&& otherMesh)
                 : aabb(std::move(otherMesh.aabb)), surfaces(std::move(otherMesh.surfaces)) {}

inline Mesh::~Mesh() 
{
    for (Surface* surface : surfaces) delete surface;
}

inline const std::pair<Surface*, float> Mesh::intersection(const Ray &ray) const
{
    float tClosest = std::numeric_limits<float>::max(); 
    Surface* closestSurface = nullptr;

    for(Surface* surface : surfaces) {
        std::pair<bool, float> intersection = surface->intersection(ray);

        if(intersection.first && intersection.second < tClosest) {
            tClosest = intersection.second;
            closestSurface = surface;
        }
    }

    return {closestSurface, tClosest};
}