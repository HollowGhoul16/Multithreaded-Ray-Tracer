#pragma once

#include "Math/Math.h"
#include "Shading/SurfaceProperties.h"
#include "Acceleration/AABB.h"
#include "Core/Mesh.h"
#include "Core/Surfaces.h"
#include "Core/Atmosphere.h"
#include "Renderer/Cameras.h"
#include "Shading/Lights.h"

struct Scene {
    Atmosphere atmosphere;
    std::vector<Mesh> meshes;
    Camera* cameras[2]; // 0 index will be orthographic, 1 will be perspective
    Camera* currentCamera;
    bool toggleCamera = 1; // Starts on perspective as default

    Scene(const Atmosphere& atm, std::vector<Mesh>&& m, Camera* c[]);

    ~Scene();

    Color getPixelColor(const float& x, const float& y, int recurse = 100) const;

    Color rayTrace(Ray& ray, int& recurse) const;

    bool castShadow(const Vec3& pointHit, const Vec3& surfaceNormal) const;

    void switchCamera();
};

#include "Scene.inl"