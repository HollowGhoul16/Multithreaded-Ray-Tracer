#include "Scene.h"

inline Scene::Scene(const Atmosphere& atm, std::vector<Mesh>&& m, Camera* c[])
                   : atmosphere(atm), meshes(std::move(m))
{
    currentCamera = cameras[toggleCamera] = c[toggleCamera];
    cameras[!toggleCamera] = c[!toggleCamera];
}

inline Scene::~Scene() 
{
    for (Camera* camera : cameras) delete camera;
}

inline Color Scene::getPixelColor(const float& x, const float& y, int recurse) const
{
    Ray cameraRay = currentCamera->getRay(x, y);
    Color color = rayTrace(cameraRay, recurse);
    color.clamp();
    color.toSRGB(); // Gamma correction

    return color;
}

inline Color Scene::rayTrace(Ray& ray, int& recurse) const
{
    float tClosestSurface = std::numeric_limits<float>::max();
    float tClosestMesh = std::numeric_limits<float>::max();
    const Mesh* closestMesh = nullptr;
    const Surface* closestSurface = nullptr;

    for(const Mesh& mesh : meshes) {
        std::pair<bool, float> meshIntersection = mesh.aabb.intersection(ray, tClosestMesh);

        if(!meshIntersection.first) continue;

        closestMesh = &mesh;
        std::pair<Surface*, float> surfaceIntersection = closestMesh->intersection(ray);
        if(!surfaceIntersection.first) continue;
        tClosestMesh = meshIntersection.second;

        if(surfaceIntersection.second < tClosestSurface) {
            tClosestSurface = surfaceIntersection.second;
            closestSurface = surfaceIntersection.first;
        }
    }

    if(!closestSurface) return atmosphere.skyModel(ray);

    Vec3 pointHit = ray.parametrize(tClosestSurface);
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

    for(const Mesh& mesh : meshes) {
        std::pair<bool, float> intersection = mesh.aabb.intersection(shadowRay, std::numeric_limits<float>::max());

        if(!intersection.first) continue;

        if(mesh.intersection(shadowRay).first) return true;
    }

    return false;
}

inline void Scene::switchCamera()
{
    toggleCamera = !toggleCamera;
    currentCamera = cameras[toggleCamera];
}