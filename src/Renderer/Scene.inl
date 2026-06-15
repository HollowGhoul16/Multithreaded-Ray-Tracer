#include "Scene.h"

inline Scene::Scene(const Atmosphere& atm, std::vector<Mesh>&& m, std::vector<Light*> l, Camera* c[])
                   : atmosphere(atm), meshes(std::move(m)), lights(std::move(l))
{
    meshes.reserve(1000);
    lights.push_back(&atmosphere.luminary);
    currentCamera = cameras[toggleCamera] = c[toggleCamera];
    cameras[!toggleCamera] = c[!toggleCamera];
}

inline Scene::~Scene() 
{
    for (Camera* camera : cameras) delete camera;
}

inline Mesh* Scene::selectMesh(const float& x, const float& y)
{
    Ray ray = currentCamera->getRay(x, y);

    Mesh* closestMesh = nullptr;
    float tClosestSurface = std::numeric_limits<float>::max();

    for(Mesh& mesh : meshes) {
        if(!mesh.AABBcontains(ray.origin)) {
            HitData meshHitData = mesh.AABBintersection(ray);
            if(!meshHitData.hit) continue;

            float tMeshWorld = mesh.transform.tToWorld(ray, meshHitData.point);
            if(tMeshWorld > tClosestSurface) continue;
        }

        HitData surfaceHitData = mesh.intersection(ray);
        if(!surfaceHitData.hit) continue;

        float tSurfaceWorld = mesh.transform.tToWorld(ray, surfaceHitData.point);
        if(tSurfaceWorld < tClosestSurface) {
            tClosestSurface = tSurfaceWorld;
            closestMesh = &mesh;
        }
    }

    return closestMesh;
}

inline std::vector<Mesh*> Scene::addMeshes(std::vector<Mesh>&& meshes)
{
    std::vector<Mesh*> newSelectedMeshes;

    for(Mesh& mesh : meshes) {
        this->meshes.push_back(std::move(mesh));
        newSelectedMeshes.push_back(&(this->meshes.back()));
    }

    return newSelectedMeshes;
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
    const Mesh* meshHit = nullptr;
    HitData hitData;

    for(const Mesh& mesh : meshes) {
        if(!mesh.AABBcontains(ray.origin)) {
            HitData meshHitData = mesh.AABBintersection(ray);
            if(!meshHitData.hit) continue;

            float tMeshWorld = mesh.transform.tToWorld(ray, meshHitData.point);
            if(tMeshWorld > tClosestSurface) continue;
        }

        HitData surfaceHitData = mesh.intersection(ray);
        if(!surfaceHitData.hit) continue;

        float tSurfaceWorld = mesh.transform.tToWorld(ray, surfaceHitData.point);
        if(tSurfaceWorld < tClosestSurface) {
            tClosestSurface = tSurfaceWorld;
            hitData = surfaceHitData;
            meshHit = &mesh;
        }
    }

    for(const Light* light : lights) {
        if(light->type == Light::Type::Point) {
            const PointLight* pt = static_cast<const PointLight*>(light);
            HitData surfaceHitData = pt->surface->intersection(ray);

            if(!surfaceHitData.hit) continue;

            if(surfaceHitData.t < tClosestSurface) {
                tClosestSurface = surfaceHitData.t;
                hitData = surfaceHitData;
            }
        }
    }

    if(!hitData.hit) return atmosphere.skyModel(ray);
    else if(hitData.material.isEmissive) return hitData.material.specularColor;

    const Transform transform = meshHit->transform;
    const Vec3 pointHit       = transform.pointToWorld(hitData.point);
    const float worldT        = (pointHit - ray.origin).dot(ray.direction);
    const Vec3 faceNormal     = transform.normalToWorld(hitData.faceNormal).normalize();
    const Vec3 shadingNormal  = transform.normalToWorld(hitData.shadingNormal).normalize();
    Material surfaceMat       = hitData.material;

    ray.cone.updateWidth(worldT);

    if(hitData.surfaceType == SurfaceType::Triangle && meshHit->texture.exists()) {
        surfaceMat = meshHit->getTextureMaterial(hitData.surfacePtr, hitData.texCoord, faceNormal, ray);
    }

    Color color = getShadedColor(surfaceMat, ray, pointHit, shadingNormal);

    if(surfaceMat.isEmissive || !surfaceMat.isGlazed || recurse <= 0) return color;

    ray.origin = pointHit + faceNormal * 0.001f;
    ray.direction = (ray.direction - (faceNormal * faceNormal.dot(ray.direction) * 2)).normalize();

    if(surfaceMat.isMirror) return rayTrace(ray, --recurse) * color * surfaceMat.specularCoeff;
    else color = color + (rayTrace(ray, --recurse) * surfaceMat.specularCoeff);

    return color;
}

inline Color Scene::getShadedColor(const Material& mat, const Ray& ray, const Vec3& point, const Vec3& normal) const
{
    if(mat.isMirror || mat.isEmissive) return mat.specularColor;

    Color finalColor;

    // Ambient Light
    Color ambient = mat.ambientColor * mat.ambientCoeff;

    finalColor = ambient;

    for(const Light* light : lights) {
        Vec3 reversedLight = -light->getDirection(point);

        // No specular and diffuse lighting if there should be shadow
        if(castShadow(light, reversedLight, point, normal)) continue;

        float intensity = light->calculateIntensity(point);

        // Diffuse Light
        Color diffuse = mat.diffuseColor * mat.diffuseCoeff * std::max(0.0f, normal.dot(reversedLight)) * intensity;

        // Specular Light
        Vec3 viewDir = -ray.direction.normalize();
        Vec3 bisector = (reversedLight + viewDir).normalize();
        Color specular = light->color * mat.specularCoeff * std::pow((std::max(0.0f, normal.dot(bisector))), mat.specularExp) * intensity;

        finalColor = finalColor + diffuse + specular;
    }

    return finalColor;
}

inline bool Scene::castShadow(const Light* light, const Vec3& lightDir, const Vec3& pointHit, const Vec3& surfaceNormal) const
{
    Ray shadowRay(pointHit + surfaceNormal * 0.001f, lightDir);
    const float distToLight = light->distanceTo(shadowRay.origin);
    float worldT = std::numeric_limits<float>::max();
    HitData hitData;

    for(const Mesh& mesh : meshes) {
        if(!mesh.AABBcontains(shadowRay.origin)) {
            hitData = mesh.AABBintersection(shadowRay);
            if(!hitData.hit) continue;

            worldT = mesh.transform.tToWorld(shadowRay, hitData.point);
            if(worldT > distToLight) continue;
        }

        hitData = mesh.intersection(shadowRay);
        if(!hitData.hit) continue;

        worldT = mesh.transform.tToWorld(shadowRay, hitData.point);
        if(worldT < distToLight) return true;
    }

    return false;
}

inline void Scene::switchCamera()
{
    toggleCamera = !toggleCamera;
    currentCamera = cameras[toggleCamera];
}