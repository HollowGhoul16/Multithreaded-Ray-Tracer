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
    Color color;
    Ray cameraRay = currentCamera->getRay(x, y);
    color = rayTrace(cameraRay, recurse);
    color.clamp();
    color.toSRGB(); // Gamma correction

    return color;
}

inline Color Scene::rayTrace(const Ray& ray, int& recurse) const
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

    if(!closestSurface) return skyModel(ray);

    Vec3 pointHit = ray.parametrize(tClosest);
    Vec3 surfaceNormal = closestSurface->normal(pointHit);
    const Material surfaceMat = closestSurface->material;
    bool shadow = castShadow(pointHit, surfaceNormal);

    Color color = surfaceMat.getColor(ray, surfaceNormal, light, shadow);

    if(!surfaceMat.glazed || recurse <= 0) return color;

    Vec3 rayDirNorm = ray.direction.normalize();
    Vec3 reflectionDirection = rayDirNorm - (surfaceNormal * surfaceNormal.dot(rayDirNorm) * 2);
    Ray reflectedRay(pointHit + surfaceNormal * 0.001f, reflectionDirection);

    color = color + (rayTrace(reflectedRay,  --recurse) * surfaceMat.specularCoeff);

    return color;
}

inline Color Scene::skyModel(const Ray& ray) const
{
    Color skyColor(100.0f, 104.0f, 255.0f);

    Color horizonColor(255.0f, 120.0f, 60.0f);

    Color sunColor = light.color;
    const float SUN_SIZE_SCALAR = 1.0f;

    Vec3 rayDir = ray.direction.normalize();
    Vec3 sunDir = -light.direction.normalize();
    float cosTheta = std::min(std::max(rayDir.dot(sunDir), 0.0f), 1.0f);

    float horizonScalar = -1 * (Vec3(0.0f, 1.0f, 0.0f).dot(rayDir) - 1);
    horizonScalar = std::pow(horizonScalar, 4.0f);

    float skyGradient = 1 + cosTheta * cosTheta;
    Color sky = (skyColor * skyGradient) * (1 - horizonScalar) + horizonColor * horizonScalar;

    float sunScalar = 0.05f + std::pow(cosTheta, 256.0f / SUN_SIZE_SCALAR);
    Color sun = sunColor * sunScalar * 2.0f;

    Color result = sky * (1 - sunScalar) + sun;

    return result;
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