#include "Mesh.h"

inline Mesh::Mesh(const std::shared_ptr<std::vector<Triangle>>& triangles, const Mat4& modelMatrix)
                 : triangles(triangles), transform(modelMatrix)
{
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(std::numeric_limits<float>::lowest());

    for(const Triangle& triangle : *(this->triangles)) {
        std::pair<Vec3, Vec3> minMaxBounds = triangle.getBounds();

        minMaxBounds.first = transform.pointToWorld(minMaxBounds.first);
        minMaxBounds.second = transform.pointToWorld(minMaxBounds.second);

        for(int i = 0; i < 3; ++i) {
            min[i] = std::min(min[i], minMaxBounds.first[i]);
            max[i] = std::max(max[i], minMaxBounds.second[i]);
        }
    }

    aabb.min = min;
    aabb.max = max;
    aabb.constructWireframe();
}

inline Mesh::Mesh(std::vector<Surface*>&& surfaces, const Mat4& modelMatrix)
                 : surfaces(std::move(surfaces)), transform(modelMatrix)
{
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(std::numeric_limits<float>::lowest());

    for(const Surface* surface : this->surfaces) {
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
      surfaces(std::move(otherMesh.surfaces)),
      transform(std::move(otherMesh.transform)),
      material(std::move(otherMesh.material))
{}

inline Mesh::~Mesh() 
{
    for (Surface* surface : surfaces) delete surface;
}

inline const HitData Mesh::intersection(const Ray &worldRay) const
{
    Ray localRay = transform.rayToLocal(worldRay);
    float tClosest = std::numeric_limits<float>::max();
    HitData finalHitData;

    if(triangles.get() != nullptr) {
        for(const Triangle& triangle : *triangles) {
            HitData currenthitData = triangle.intersection(localRay);

            if(currenthitData.hit && currenthitData.t < tClosest) {
                tClosest = currenthitData.t;
                finalHitData = currenthitData;
            }
        }
    }

    for(const Surface* surface : surfaces) {
        HitData currenthitData = surface->intersection(localRay);

        if(currenthitData.hit && currenthitData.t < tClosest) {
            tClosest = currenthitData.t;
            finalHitData = currenthitData;
        }
    }

    if(wireframeAABB) {
        for(const Rectangle& rect : aabb.wireframe) {
            HitData currenthitData = rect.intersection(worldRay);

            if(currenthitData.hit && currenthitData.t < tClosest) {
                tClosest = currenthitData.t;
                finalHitData = currenthitData;
            }
        }
    }

    finalHitData.point = transform.pointToWorld(finalHitData.point);
    finalHitData.faceNormal = transform.normalToWorld(finalHitData.faceNormal);
    finalHitData.shadingNormal = transform.normalToWorld(finalHitData.shadingNormal);
    finalHitData.material = material;

    return finalHitData;
}