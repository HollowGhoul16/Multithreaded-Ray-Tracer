#include "SurfaceProperties.h"

// Color struct

constexpr Color::Color(const float& r, const float& g, const float& b)
                      : r(r), g(g), b(b) {};

inline Color Color::operator*(const float& coeff) const 
{
    return Color(r * coeff, g * coeff, b * coeff);
}

inline Color Color::operator/(const float& coeff) const 
{
    return Color(r / coeff, g / coeff, b / coeff);
}

inline Color Color::operator*(const Color& otherColor) const 
{
    return Color(r * otherColor.r, g * otherColor.g, b * otherColor.b);
}

inline Color Color::operator+(const Color& otherColor) const 
{
    return Color(r + otherColor.r, g + otherColor.g, b + otherColor.b);
}

inline void Color::clamp() 
{
    r = std::min(r, 255.0f);
    r = std::max(r, 0.0f);
    g = std::min(g, 255.0f);
    g = std::max(g, 0.0f);
    b = std::min(b, 255.0f);
    b = std::max(b, 0.0f);
}

// Material struct

constexpr Material::Material(
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
    Color specular = specularColor * specularCoeff * std::pow((std::max(0.0f, normal.dot(bisector))), specularExp) * light.intensity;

    return ambient + diffuse + specular;
}