#pragma once

#include "Shading/Lights.h"
#include "Math/Math.h"

struct Atmosphere {
    DirectionalLight luminary;
    float luminarySize;
    Color skyColor, horizonColor;

    Color (Atmosphere::*selectedModel)(const Ray&) const; // Function pointer dependent on bool from constructor

    Atmosphere(
        const DirectionalLight& luminaryDir,
        const float& luminarySize,
        const Color& skyColor,
        const Color& horColor,
        const bool& isSun
    );

    Color skyModel(const Ray& ray) const; // Calls selected model

    Color sunModel(const Ray& ray) const;

    Color moonModel(const Ray& ray) const;
};

#include "Atmosphere.inl"