#pragma once

#include "Math/Math.h"
#include "Shading/SurfaceProperties.h"
#include "Core/Surfaces.h"
#include "Renderer/Cameras.h"
#include "Shading/Lights.h"
#include <limits>
#include <string>
#include <vector>

struct Scene {
    DirectionalLight light;
    std::vector<Surface*> surfaces;
    Camera* cameras[2]; // 0 index will be orthographic, 1 will be perspective
    Camera* currentCamera;
    bool toggleCamera = 1; // Starts on perspective as default

    Scene(const DirectionalLight& dl, std::vector<Surface*>& s, Camera* c[]);

    ~Scene();

    Color getPixelColor(const float& x, const float& y, int recurse = 1) const;

    Color reflectionColor(const Ray& ray, int& recurse) const;

    bool castShadow(const Vec3& pointHit, const Vec3& surfaceNormal) const;

    void switchCamera();
};

#include "Scene.inl"