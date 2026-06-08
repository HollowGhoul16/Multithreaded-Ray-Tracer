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

inline Mesh::Mesh(const Mesh& otherMesh)
    : aabb(otherMesh.aabb),
      triangles(otherMesh.triangles),
      surfaces(otherMesh.surfaces),
      transform(otherMesh.transform),
      texture(otherMesh.texture),
      material(otherMesh.material),
      cullMode(otherMesh.cullMode)
{}

inline Mesh::Mesh(Mesh&& otherMesh)
    : aabb(std::move(otherMesh.aabb)),
      triangles(std::move(otherMesh.triangles)),
      surfaces(std::move(otherMesh.surfaces)),
      transform(std::move(otherMesh.transform)),
      texture(std::move(otherMesh.texture)),
      material(std::move(otherMesh.material)),
      cullMode(std::move(otherMesh.cullMode))
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
    Vec3 localPoint = transform.pointToLocal(worldPoint);
    return aabb.contains(localPoint);
}

inline const bool Mesh::cull(const Vec3& rayDir, const Vec3& surfaceNormal) const
{
    switch(cullMode) {
               case CullMode::Off:            return false;
        break; case CullMode::Front:          return rayDir.dot(surfaceNormal) < 0;
        break; case CullMode::Back:           return rayDir.dot(surfaceNormal) > 0;
        break; case CullMode::Front_And_Back: return true;
        break; default:                       return false;
    }
}

inline const HitData Mesh::intersection(Ray& worldRay) const
{
    Ray localRay = transform.rayToLocal(worldRay);
    float tClosest = std::numeric_limits<float>::max();
    HitData finalHitData;

    if(triangles.get() != nullptr) {
        for(const Triangle& triangle : *triangles) {
            if(cull(localRay.direction, triangle.normalVec)) continue;
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

    if(!finalHitData.hit) return finalHitData; // Prevent unneccessary computations

    finalHitData.point = transform.pointToWorld(finalHitData.point);
    finalHitData.t = (finalHitData.point - worldRay.origin).dot(worldRay.direction); // Compute world t value
    finalHitData.faceNormal = transform.normalToWorld(finalHitData.faceNormal);
    finalHitData.shadingNormal = transform.normalToWorld(finalHitData.shadingNormal);
    if(!hitBox) finalHitData.material = material; // Make sure to keep AABB material

    worldRay.cone.updateWidth(finalHitData.t);

    if(finalHitData.surfaceType == SurfaceType::Triangle && texture.exists() && finalHitData.texCoord.x != -1 && finalHitData.texCoord.y != -1) {
        float lambda = static_cast<const Triangle*>(finalHitData.surface)->getLODConstant(transform.modelMatrix);
        lambda += std::log2(worldRay.cone.width);
        lambda += 0.5 * log2(texture.baseLevel.width * texture.baseLevel.height);
        lambda -= std::log2(std::abs(worldRay.direction.dot(finalHitData.faceNormal)));
        lambda -= 0.5f; // Constant to prevent slight overblur

        Color texColor = texture.sample(finalHitData.texCoord.x, finalHitData.texCoord.y, lambda);
        finalHitData.material.ambientColor = texColor;
        finalHitData.material.diffuseColor = texColor;
    }

    // AABB is the inverted color of the mesh
    if(hitBox && selected) { // TODO: Fix this weird logic for materials when selected or debug mode
        finalHitData.material = material;
        finalHitData.material.ambientColor  = (finalHitData.material.ambientColor  - 1) * -1;
        finalHitData.material.diffuseColor  = (finalHitData.material.diffuseColor  - 1) * -1;
        finalHitData.material.specularColor = (finalHitData.material.specularColor - 1) * -1;
    }

    return finalHitData;
}

inline Mesh Mesh::duplicate() const
{
    Mesh newMesh(*this);
    const Vec4 OFFSET(100, 100, 100, 0);
    newMesh.transform.modelMatrix.w = newMesh.transform.modelMatrix.w + OFFSET;
    newMesh.transform.update();

    return newMesh;
}

inline void Mesh::applyTexture(const TextureData& textureData, const Texture::SampleFilter& sampleFilter)
{
    texture = Texture(textureData, sampleFilter);
}

inline void Mesh::applyTransform(const Mat4& transformation)
{
    transform.applyTransform(transformation);
}

inline void Mesh::setCullingMode(const CullMode& cullMode)
{
    this->cullMode = cullMode;
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