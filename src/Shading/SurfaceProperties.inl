#include "SurfaceProperties.h"

// Material struct

inline Material::Material(
    const Color& ac, 
    const Color& dc, 
    const Color& sc, 
    const float& ace, 
    const float& dce, 
    const float& sce, 
    const float& se, 
    const bool& g
    )
    : ambientColor(ac), 
      diffuseColor(dc), 
      specularColor(sc), 
      ambientCoeff(ace), 
      diffuseCoeff(dce), 
      specularCoeff(sce), 
      specularExp(se), 
      glazed(g) 
{}

inline Color Material::getColor(
    const Ray& ray, 
    const Vec3& normal, 
    const DirectionalLight& light,
    const bool& shadow
) const
{
    Vec3 reversedLight = -light.direction; 

    // Ambient Light
    Color ambient = ambientColor * ambientCoeff;

    if(shadow) return ambient; // No specular and diffuse lighting if there should be shadow

    // Diffuse Light
    Color diffuse = diffuseColor * diffuseCoeff * std::max(0.0f, normal.dot(reversedLight)) * light.intensity;

    // Specular Light
    Vec3 viewDir = -ray.direction.normalize();
    Vec3 bisector = (reversedLight + viewDir).normalize();
    Color specular = light.color * specularCoeff * std::pow((std::max(0.0f, normal.dot(bisector))), specularExp) * light.intensity;

    return ambient + diffuse + specular;
}