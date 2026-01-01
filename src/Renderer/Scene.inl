#include "Scene.h"

inline Scene::Scene(const DirectionalLight& dl, std::vector<Surface*>& s, Camera* c[])
                   : light(dl), surfaces(s) 
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
    Color color; // Gets initialized to black (0, 0, 0)
    Ray cameraRay = currentCamera->getRay(x, y);

    float tClosest = std::numeric_limits<float>::max(); 
    const Surface* closestSurface = nullptr;

    for(const Surface* surface : surfaces) {
        std::pair<bool, float> intersection = surface->intersection(cameraRay);

        if(intersection.first && intersection.second < tClosest) {
            tClosest = intersection.second;
            closestSurface = surface;
        }
    }

    if(!closestSurface) return color; // If we did not hit return a black pixel

    Vec3 pointHit = cameraRay.parametrize(tClosest);
    Vec3 surfaceNormal = closestSurface->normal(pointHit);
    Material surfaceMat = closestSurface->material;
    bool shadow = castShadow(pointHit, surfaceNormal);

    color = surfaceMat.getColor(cameraRay, surfaceNormal, light, shadow);

    if(!surfaceMat.glazed || recurse <= 0) return color;

    Vec3 reflectionDirection = cameraRay.direction - (surfaceNormal * surfaceNormal.dot(cameraRay.direction) * 2);
    Ray reflectedRay(pointHit + surfaceNormal * 0.001f, reflectionDirection); // Adds a little buffer space so we do not hit the same point again

    color = color + (reflectionColor(reflectedRay, --recurse) * surfaceMat.specularCoeff * surfaceMat.specularColor / 255.0f);
    color.clamp();

    return color;
}

inline Color Scene::reflectionColor(const Ray& ray, int& recurse) const
{
    Color color;

    float tClosest = std::numeric_limits<float>::max(); 
    const Surface* closestSurface = nullptr;

    for(const Surface* surface : surfaces) {
        std::pair<bool, float> intersection = surface->intersection(ray);

        if(intersection.first && intersection.second < tClosest) {
            tClosest = intersection.second;
            closestSurface = surface;
        }
    }

    if(!closestSurface) return color; // If we did not hit return a black pixel

    Vec3 pointHit = ray.parametrize(tClosest);
    Vec3 surfaceNormal = closestSurface->normal(pointHit);
    const Material surfaceMat = closestSurface->material;
    bool shadow = castShadow(pointHit, surfaceNormal);

    color = surfaceMat.getColor(ray, surfaceNormal, light, shadow);

    if(!surfaceMat.glazed || recurse <= 0) return color;

    Vec3 rayDirNorm = ray.direction.normalize();
    Vec3 reflectionDirection = rayDirNorm - (surfaceNormal * surfaceNormal.dot(rayDirNorm) * 2);
    Ray reflectedRay(pointHit + surfaceNormal * 0.001f, reflectionDirection);

    color = color + (reflectionColor(reflectedRay, --recurse) * surfaceMat.specularCoeff * surfaceMat.specularColor / 255.0f);

    return color;
}

inline bool Scene::castShadow(const Vec3& pointHit, const Vec3& surfaceNormal) const
{
    Ray shadowRay(pointHit + surfaceNormal * 0.001f, -light.direction);

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