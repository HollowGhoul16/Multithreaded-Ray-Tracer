#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Mesh.h"

// If .obj file has: P = Position, T = Texture, N = Normal
enum ObjFormat {
    P,
    PT,
    PN,
    PTN,
    Invalid
};

Mesh loadObj(const std::string& path, const Matrix4& modelMatrix, const Material& mat); // Supports only triangles