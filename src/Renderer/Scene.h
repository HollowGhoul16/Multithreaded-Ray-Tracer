#pragma once

#include "Math/Math.h"
#include "Shading/SurfaceProperties.h"
#include "Acceleration/AABB.h"
#include "Geometry/Mesh.h"
#include "Core/Atmosphere.h"
#include "Renderer/Cameras.h"
#include "Shading/Lights.h"

struct Scene {
    Atmosphere atmosphere;
    std::vector<Mesh> meshes;
    std::vector<Light*> lights;
    Camera* cameras[2]; // 0 index will be orthographic, 1 will be perspective
    Camera* currentCamera;
    bool toggleCamera = 1; // Starts on perspective as default

    Scene(const Atmosphere& atm, std::vector<Mesh>&& m, std::vector<Light*> l, Camera* c[]);

    ~Scene();

    Mesh* selectMesh(const float& x, const float& y);

    std::vector<Mesh*> addMeshes(std::vector<Mesh>&& meshes);

    Color getPixelColor(const float& x, const float& y, int recurse = 100) const;

    Color rayTrace(Ray& ray, int& recurse) const;

    Color getColor(const Material& mat, const Ray& ray, const Vec3& point, const Vec3& normal) const;

    bool castShadow(const Light* light, const Vec3& lightDir, const Vec3& pointHit, const Vec3& surfaceNormal) const;

    void switchCamera();
};

#include "Scene.inl"