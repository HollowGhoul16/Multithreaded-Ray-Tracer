#include "ObjParser.h"

MeshData parseObj(const std::string& path)
{
    ObjFormat fileFormat = ObjFormat::Invalid;
    std::ifstream obj;
    std::string buffer;

    float xMax, yMax, zMax;
    xMax = yMax = zMax = std::numeric_limits<float>::lowest();

    float xMin, yMin, zMin;
    xMin = yMin = zMin = std::numeric_limits<float>::max();

    std::vector<float> verts;
    std::vector<float> vertexTextCoords;
    std::vector<float> vertexNormals;

    std::vector<Triangle> triangles;
    Vec3 triangleVerts[3];
    Vec2 textureCoords[3];
    Vec3 triangleNorms[3];

    obj.open(path);

    // Determine what attributes are in file
    while(obj >> buffer) {
        if(buffer == "#" || buffer != "f") continue;

        obj >> buffer;
        int slashCount = 0;
        for(size_t i = 0; i < buffer.size(); ++i) if(buffer.at(i) == '/') slashCount++;

        if(slashCount == 0) {
            fileFormat = ObjFormat::P; // Only vertex coordinates
        }
        else if (slashCount == 1) {
            fileFormat = ObjFormat::PT; // Only vertex/texture coordinates
        }
        else if (slashCount == 2) {
            int firstSlash = buffer.find_first_of("/");
            int secondSlash = buffer.find_last_of("/");

            if(firstSlash + 1 == secondSlash) {
                fileFormat = ObjFormat::PN; // Only vertex coordinates/normals
            }
            else {
                fileFormat = ObjFormat::PTN; // Vertex coordinates/texture/normals
            }
        }
        else {
            fileFormat = ObjFormat::Invalid;
        }

        break;
    }

    if(fileFormat == ObjFormat::Invalid) {
        std::cerr << "Error when importing mesh from path: " << path << "\n";
        std::cerr << "Returning from function with empty mesh.\n";
        return MeshData(std::move(std::make_shared<std::vector<Triangle>>()));;
    }

    obj.clear();
    obj.seekg(0, std::ios::beg);
    
    while(obj >> buffer) {
        if(buffer == "#") {
            std::getline(obj, buffer);
        }
        else if(buffer == "v") {
            for(int i = 0; i < 3; ++i) {
                obj >> buffer;
                verts.push_back(std::stof(buffer));
            }
        }
        else if(buffer == "vt") {
            for(int i = 0; i < 2; ++i) {
                obj >> buffer;
                vertexTextCoords.push_back(std::stof(buffer)); 
            }
        }
        else if (buffer == "vn") {
            for(int i = 0; i < 3; ++i) {
                obj >> buffer;
                vertexNormals.push_back(std::stof(buffer));
            }
        }
        else if(buffer == "f") {
            for(int i = 0; i < 3; ++i) {
                obj >> buffer;

                int coordIndex, textCoordIndex, normalIndex;
                int firstSlash = buffer.find_first_of("/");
                int secondSlash = buffer.find_last_of("/");

                Vec3 v, n;
                Vec2 t;

                switch(fileFormat) {
                    case ObjFormat::P:
                        coordIndex = (std::stoi(buffer) - 1) * 3;

                        v = Vec3(verts[coordIndex], verts[coordIndex + 1], verts[coordIndex + 2]);

                        triangleVerts[i] = v;

                    break; case ObjFormat::PT:
                        coordIndex = (std::stoi(buffer.substr(0, firstSlash)) - 1) * 3;
                        textCoordIndex = (std::stoi(buffer.substr(firstSlash + 1, buffer.size() - firstSlash - 1)) - 1) * 3;

                        v = Vec3(verts[coordIndex], verts[coordIndex + 1], verts[coordIndex + 2]);
                        t = Vec2(vertexTextCoords[textCoordIndex], vertexTextCoords[textCoordIndex + 1]);
                        
                        triangleVerts[i] = v;
                        textureCoords[i] = t;

                    break; case ObjFormat::PN:
                        coordIndex = (std::stoi(buffer.substr(0, firstSlash)) - 1) * 3;
                        normalIndex = (std::stoi(buffer.substr(secondSlash + 1, buffer.size() - secondSlash - 1)) - 1) * 3;

                        v = Vec3(verts[coordIndex], verts[coordIndex + 1], verts[coordIndex + 2]);
                        n = Vec3(vertexNormals[normalIndex], vertexNormals[normalIndex + 1], vertexNormals[normalIndex + 2]);

                        triangleVerts[i] = v;
                        triangleNorms[i] = n.normalize();

                    break; case ObjFormat::PTN:
                        coordIndex = (std::stoi(buffer.substr(0, firstSlash)) - 1) * 3;
                        textCoordIndex = (std::stoi(buffer.substr(firstSlash + 1, secondSlash - firstSlash - 1)) - 1) * 2;
                        normalIndex = (std::stoi(buffer.substr(secondSlash + 1, buffer.size() - secondSlash - 1)) - 1) * 3;

                        v = Vec3(verts[coordIndex], verts[coordIndex + 1], verts[coordIndex + 2]);
                        t = Vec2(vertexTextCoords[textCoordIndex], vertexTextCoords[textCoordIndex + 1]);
                        n = Vec3(vertexNormals[normalIndex], vertexNormals[normalIndex + 1], vertexNormals[normalIndex + 2]);

                        triangleVerts[i] = v;
                        textureCoords[i] = t;
                        triangleNorms[i] = n.normalize();

                    break; default: break;
                }
            }

            // If no vertex normals, default to using face normals (geometric shading)
            if(fileFormat == ObjFormat::P || fileFormat == ObjFormat::PT) {
                Vec3 geometricNormal = (triangleVerts[1] - triangleVerts[0]).cross((triangleVerts[2] - triangleVerts[1]));
                for(int i = 0; i < 3; ++i) triangleNorms[i] = geometricNormal;
            }

            Triangle triangle = Triangle(triangleVerts, textureCoords, triangleNorms, Material());
            triangles.push_back(triangle);

            for(int i = 0; i < 3; ++i) {
                xMax = std::max(xMax, triangleVerts[i].x);
                yMax = std::max(yMax, triangleVerts[i].y);
                zMax = std::max(zMax, triangleVerts[i].z);

                xMin = std::min(xMin, triangleVerts[i].x);
                yMin = std::min(yMin, triangleVerts[i].y);
                zMin = std::min(zMin, triangleVerts[i].z);
            }
        }
    }

    obj.close();

    float xNormalize = std::max(std::abs(xMax), std::abs(xMin));
    float yNormalize = std::max(std::abs(yMax), std::abs(yMin));
    float zNormalize = std::max(std::abs(zMax), std::abs(zMin));

    float max = std::abs(std::max(std::max(xNormalize, yNormalize), zNormalize));

    // Normalize each vertex dependent on longest "half"-axis (either the negative or positive side)
    for(Triangle& triangle : triangles) {
        for(int i = 0; i < 3; ++i) {
            triangle.vertices[i].x /= max;
            triangle.vertices[i].y /= max;
            triangle.vertices[i].z /= max;
        }

        for(int i = 0; i < 3; ++i) triangle.edges[i] = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
    }

    std::shared_ptr<std::vector<Triangle>> trianglesPtr = std::make_shared<std::vector<Triangle>>(std::move(triangles));

    MeshData meshData(std::move(trianglesPtr));

    return meshData;
}