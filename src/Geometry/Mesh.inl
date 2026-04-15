#include "Mesh.h"

inline Mesh::Mesh(const std::shared_ptr<std::vector<Triangle>>& triangles, const Mat4& modelMatrix)
                 : triangles(triangles), transform(modelMatrix)
{
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(std::numeric_limits<float>::lowest());

    for(const Triangle& triangle : *(this->triangles)) {
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

inline const HitData Mesh::AABBintersection(const Ray& worldRay, const float& tClosestSurface) const
{
    Ray localRay = transform.rayToLocal(worldRay);
    return aabb.intersection(localRay, tClosestSurface);
}

inline const bool Mesh::AABBcontains(const Vec3& worldPoint) const
{
    return aabb.contains(transform.pointToLocal(worldPoint));
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

    bool hitBox = false;
    if(wireframeAABB || selected) {
        for(const Rectangle& rect : aabb.wireframe) {
            HitData currenthitData = rect.intersection(localRay);

            if(currenthitData.hit && currenthitData.t <= tClosest + 1e-4f) { // Prioritize AABB for debugging
                tClosest = currenthitData.t;
                finalHitData = currenthitData;
                hitBox = true;
            }
        }
    }

    finalHitData.point = transform.pointToWorld(finalHitData.point);
    finalHitData.faceNormal = transform.normalToWorld(finalHitData.faceNormal);
    finalHitData.shadingNormal = transform.normalToWorld(finalHitData.shadingNormal);
    if(!hitBox) finalHitData.material = material; // Make sure to keep AABB material

    // AABB is the inverted color of the mesh
    if(hitBox && selected) { // TODO: Fix this weird logic for matierls when selected or debug mode
        finalHitData.material = material;
        finalHitData.material.ambientColor = (finalHitData.material.ambientColor - 1) * -1;
        finalHitData.material.diffuseColor = (finalHitData.material.diffuseColor - 1) * -1;
        finalHitData.material.specularColor = (finalHitData.material.specularColor - 1) * -1;
    }

    return finalHitData;
}

inline Mesh Mesh::duplicate() const
{
    Mat4 newModelMatrix = this->transform.modelMatrix;

    thread_local Vec4 OFFSET = Vec4(100, 100, 100, 0);
    newModelMatrix.w = newModelMatrix.w + OFFSET;

    Mesh newMesh(this->triangles, newModelMatrix);
    newMesh.material = this->material;

    return newMesh;
}

inline void Mesh::applyTransform(const Mat4& transformation)
{
    transform.applyTransform(transformation);
}

inline void Mesh::toggleSelected()
{
    selected = !selected;
}

inline void Mesh::markSelected()
{
    selected = true;
}

inline void Mesh::markDeselected()
{
    selected = false;
}