#include "Scene.h"

inline Scene::Scene(const Atmosphere& atm, std::vector<Surface*>& s, Camera* c[])
                   : atmosphere(atm), surfaces(s) 
{
    currentCamera = cameras[toggleCamera] = c[toggleCamera];
    cameras[!toggleCamera] = c[!toggleCamera];
}

inline Scene::~Scene() 
{
    for (Surface* surface : surfaces) delete surface;
    for (Camera* camera : cameras) delete camera;
}

inline Color Scene::getPixelColor(const float& x, const float& y, int recurse) const
{
    Color color;
    Ray cameraRay = currentCamera->getRay(x, y);
    color = rayTrace(cameraRay, recurse);
    color.clamp();
    color.toSRGB(); // Gamma correction

    return color;
}

inline Color Scene::rayTrace(Ray& ray, int& recurse) const
{
    float tClosest = std::numeric_limits<float>::max(); 
    const Surface* closestSurface = nullptr;

    for(const Surface* surface : surfaces) {
        std::pair<bool, float> intersection = surface->intersection(ray);

        if(intersection.first && intersection.second < tClosest) {
            tClosest = intersection.second;
            closestSurface = surface;
        }
    }

    if(!closestSurface) return atmosphere.skyModel(ray);

    Vec3 pointHit = ray.parametrize(tClosest);
    Vec3 surfaceNormal = closestSurface->normal(ray, pointHit);
    const Material surfaceMat = closestSurface->material;

    Color color;
    bool shadow;
    if(!surfaceMat.isMirror) {
        shadow = castShadow(pointHit, surfaceNormal);
        color = surfaceMat.getColor(ray, surfaceNormal, atmosphere.sun, shadow);
    }

    if(!surfaceMat.isGlazed || recurse <= 0) return color;

    Vec3 rayDirNorm = ray.direction.normalize();
    ray.origin = pointHit + surfaceNormal * 0.001f;
    ray.direction = rayDirNorm - (surfaceNormal * surfaceNormal.dot(rayDirNorm) * 2);

    if(surfaceMat.isMirror) return rayTrace(ray,  --recurse) * surfaceMat.specularColor * surfaceMat.specularCoeff;
    else color = color + (rayTrace(ray,  --recurse) * surfaceMat.specularCoeff);

    return color;
}

inline bool Scene::castShadow(const Vec3& pointHit, const Vec3& surfaceNormal) const
{
    Ray shadowRay(pointHit + surfaceNormal * 0.001f, -atmosphere.sun.direction);

    for(const Surface* surface : surfaces) {
        std::pair<bool, float> intersection = surface->intersection(shadowRay);

        if(intersection.first) return true;
    }

    return false;
}

inline void Scene::switchCamera()
{
    toggleCamera = !toggleCamera;
    currentCamera = cameras[toggleCamera];
}