#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "Resources/MeshData.h"

// If .obj file has: P = Position, T = Texture, N = Normal
enum class ObjFormat {
    P,
    PT,
    PN,
    PTN,
    Invalid
};

MeshData parseObj(const std::string& path); // Supports only triangles