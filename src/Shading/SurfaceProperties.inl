#include "SurfaceProperties.h"

// Material struct

inline Material::Material(const Color& color)
    : specularColor(color),
      isEmissive(true)
{}

inline Material::Material(const Color& color, const float& spe_coeff)
    : specularColor(color),
      specularCoeff(spe_coeff),
      isGlazed(true),
      isMirror(true)
{}

inline Material::Material(
    const Color &ac,
    const Color &dc,
    const Color &sc,
    const float &ace,
    const float &dce,
    const float &sce,
    const float &se,
    const bool &g)
    : ambientColor(ac),
      diffuseColor(dc),
      specularColor(sc),
      ambientCoeff(ace),
      diffuseCoeff(dce),
      specularCoeff(sce),
      specularExp(se),
      isGlazed(g),
      isMirror(false)
{}