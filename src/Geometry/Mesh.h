#pragma once

#include <vector>
#include <limits>
#include <algorithm>
#include <memory>

#include "Acceleration/AABB.h"
#include "Shading/Texture.h"
#include "Transform.h"
#include "Surfaces.h"

struct Mesh {
    AABB aabb;
    std::shared_ptr<std::vector<Triangle>> triangles;
    std::vector<Surface*> surfaces;
    Transform transform;
    Texture texture;
    Material material;
    CullMode cullMode = CullMode::Back;
    bool selected = false;
    static bool wireframeAABB;

    Mesh() = default;

    Mesh(const std::shared_ptr<std::vector<Triangle>>& triangles, const Mat4& modelMatrix = Mat4::identity());

    Mesh(std::vector<Surface*>&& surfaces, const Mat4& modelMatrix = Mat4::identity());

    Mesh(const Mesh& otherMesh);

    Mesh(Mesh&& otherMesh);

    ~Mesh();

    const HitData AABBintersection(const Ray& worldRay) const;

    const bool AABBcontains(const Vec3& worldPoint) const;

    const bool cull(const Vec3& rayDir, const Vec3& surfaceNormal) const;

    const HitData intersection(Ray& worldRay) const;

    const Material getTextureMaterial(const Surface* surface, const Vec2& texCoord, const Vec3& faceNormal, const Ray& ray) const;

    Mesh duplicate() const;

    void applyTexture(const TextureData& textureData, const Texture::SampleFilter& sampleFilter);

    void applyTransform(const Mat4& transformation);

    void setCullingMode(const CullMode& cullMode);

    static void toggleDebug();

    void toggleSelected();

    void markSelected();

    void markDeselected();
};

#include "Mesh.inl"