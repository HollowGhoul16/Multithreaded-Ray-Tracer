#include "SurfaceProperties.h"

// Color struct

inline Color::Color(const float& r, const float& g, const float& b)
                   : r(std::pow(r / 255, 2.2)),
                     g(std::pow(g / 255, 2.2)),
                     b(std::pow(b / 255, 2.2))
                   {}

inline Color Color::operator*(const float& coeff) const 
{
    Color color = Color();
    color.r = r * coeff;
    color.g = g * coeff;
    color.b = b * coeff;
    return color;
}

inline Color Color::operator/(const float& coeff) const 
{
    Color color = Color();
    color.r = r / coeff;
    color.g = g / coeff;
    color.b = b / coeff;
    return color;
}

inline Color Color::operator*(const Color& otherColor) const 
{
    Color color = Color();
    color.r = r * otherColor.r;
    color.g = g * otherColor.g;
    color.b = b * otherColor.b;
    return color;
}

inline Color Color::operator+(const Color& otherColor) const 
{
    Color color = Color();
    color.r = r + otherColor.r;
    color.g = g + otherColor.g;
    color.b = b + otherColor.b;
    return color;
}

// Go from linear [0, 1] range to [0, 255] for image display
inline void Color::toSRGB() {
    r = 255 * std::pow(r, .454);
    g = 255 * std::pow(g, .454);
    b = 255 * std::pow(b, .454);
}

inline void Color::clamp() 
{
    r = std::min(r, 1.0f);
    r = std::max(r, 0.0f);
    g = std::min(g, 1.0f);
    g = std::max(g, 0.0f);
    b = std::min(b, 1.0f);
    b = std::max(b, 0.0f);
}

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
    Color specular = specularColor * specularCoeff * std::pow((std::max(0.0f, normal.dot(bisector))), specularExp) * light.intensity;

    return ambient + diffuse + specular;
}