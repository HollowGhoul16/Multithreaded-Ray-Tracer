#pragma once

#include <variant>

#include "Shading/Cubemap.h"
#include "Shading/Lights.h"
#include "Math/Math.h"

struct Atmosphere {
    DirectionalLight luminary;

    struct Procedural {
        float luminarySize;
        Color skyColor, horizonColor;

        Procedural(const float& ls, const Color& sc, const Color& hc)
                  : luminarySize(ls), skyColor(sc), horizonColor(hc) {}
    };

    std::variant<std::monostate, Procedural, Cubemap> sky;

    Color (Atmosphere::*selectedModel)(const Ray&) const; // Function pointer dependent on constructor

    Atmosphere(
        const DirectionalLight& luminaryDir,
        const float& luminarySize,
        const Color& skyColor,
        const Color& horColor,
        const bool& isSun
    );

    Atmosphere(
        const DirectionalLight& luminaryDir,
        const Cubemap& cubeMap
    );

    Color skyModel(const Ray& ray) const; // Calls selected model

    Color skybox(const Ray& ray) const;

    Color sunModel(const Ray& ray) const;

    Color moonModel(const Ray& ray) const;
};

#include "Atmosphere.inl"