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
    HitData hitData;

    for(const Mesh& mesh : meshes) {
        HitData meshHitData = mesh.aabb.intersection(ray, tClosestMesh);

        if(!meshHitData.hit) continue;

        HitData surfaceHitData = mesh.intersection(ray);
        if(!surfaceHitData.hit) continue;

        if(surfaceHitData.t < tClosestSurface) {
            tClosestMesh = meshHitData.t;
            tClosestSurface = surfaceHitData.t;
            hitData = surfaceHitData;
        }
    }

    if(!hitData.hit) return atmosphere.skyModel(ray);

    const Vec3 pointHit = hitData.point;
    const Vec3 faceNormal = hitData.faceNormal;
    const Material surfaceMat = hitData.material;

    Color color;
    if(!surfaceMat.isMirror) {
        bool shadow = castShadow(pointHit, faceNormal);
        color = surfaceMat.getColor(ray, hitData.shadingNormal, atmosphere.sun, shadow);
    }

    if(!surfaceMat.isGlazed || recurse <= 0) return color;

    const Vec3 rayDirNorm = ray.direction.normalize();
    ray.origin = pointHit + faceNormal * 0.001f;
    ray.direction = rayDirNorm - (faceNormal * faceNormal.dot(rayDirNorm) * 2);

    if(surfaceMat.isMirror) return rayTrace(ray,  --recurse) * surfaceMat.specularColor * surfaceMat.specularCoeff;
    else color = color + (rayTrace(ray,  --recurse) * surfaceMat.specularCoeff);

    return color;
}

inline bool Scene::castShadow(const Vec3& pointHit, const Vec3& surfaceNormal) const
{
    const Ray shadowRay(pointHit + surfaceNormal * 0.001f, -atmosphere.sun.direction);
    HitData hitData;

    for(const Mesh& mesh : meshes) {
        hitData = mesh.aabb.intersection(shadowRay, std::numeric_limits<float>::max());

        if(!hitData.hit) continue;

        if(mesh.intersection(shadowRay).hit) return true;
    }

    return false;
}

inline void Scene::switchCamera()
{
    toggleCamera = !toggleCamera;
    currentCamera = cameras[toggleCamera];
}