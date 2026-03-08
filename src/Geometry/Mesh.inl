#include "Mesh.h"

inline Mesh::Mesh(std::vector<Triangle>&& triangles)
                 : triangles(triangles)
{
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(std::numeric_limits<float>::lowest());

    for(const Triangle& triangle : triangles) {
        std::pair<Vec3, Vec3> minMaxBounds = triangle.getBounds();

        for(int i = 0; i < 3; ++i) {
            min[i] = std::min(min[i], minMaxBounds.first[i]);
            max[i] = std::max(max[i], minMaxBounds.second[i]);
        }
    }

    aabb.min = min;
    aabb.max = max;
    aabb.constructWireframe();
}

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
    aabb.constructWireframe();
}

inline Mesh::Mesh(Mesh&& otherMesh)
    : aabb(std::move(otherMesh.aabb)),
      triangles(std::move(otherMesh.triangles)),
      surfaces(std::move(otherMesh.surfaces))
{}

inline Mesh::~Mesh() 
{
    for (Surface* surface : surfaces) delete surface;
}

inline const HitData Mesh::intersection(const Ray &ray) const
{
    float tClosest = std::numeric_limits<float>::max();
    HitData finalHitData;

    for(const Triangle& triangle : triangles) {
        HitData currenthitData = triangle.intersection(ray);

        if(currenthitData.hit && currenthitData.t < tClosest) {
            tClosest = currenthitData.t;
            finalHitData = currenthitData;
        }
    }

    for(const Surface* surface : surfaces) {
        HitData currenthitData = surface->intersection(ray);

        if(currenthitData.hit && currenthitData.t < tClosest) {
            tClosest = currenthitData.t;
            finalHitData = currenthitData;
        }
    }

    if(wireframeAABB) {
        for(const Rectangle& rect : aabb.wireframe) {
            HitData currenthitData = rect.intersection(ray);

            if(currenthitData.hit && currenthitData.t < tClosest) {
                tClosest = currenthitData.t;
                finalHitData = currenthitData;
            }
        }
    }

    return finalHitData;
}