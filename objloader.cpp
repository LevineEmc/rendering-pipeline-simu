#include "objloader.h"

#include <fstream>
#include <sstream>

bool OBJLoader::load(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            Vec3 v;

            ss >> v.x >> v.y >> v.z;

            vertices.push_back(v);
        }

        else if (prefix == "f")
        {
            Face f;

            std::vector<std::string> tokens;

            std::string token;

            while (ss >> token)
            {
                tokens.push_back(token);
            }

            std::vector<int> indices;

            for (const auto& t : tokens)
            {
                std::stringstream ps(t);

                std::string indexStr;

                std::getline(ps, indexStr, '/');

                indices.push_back(std::stoi(indexStr) - 1);
            }

            for (int i = 1; i < indices.size() - 1; i++)
            {
                Face f;

                f.v0 = indices[0];
                f.v1 = indices[i];
                f.v2 = indices[i + 1];

                faces.push_back(f);
            }
        }
    }
    normals.resize(vertices.size());

    for (const auto& face : faces)
    {
        Vec3 v0 = vertices[face.v0];
        Vec3 v1 = vertices[face.v1];
        Vec3 v2 = vertices[face.v2];

        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v0;

        Vec3 faceNormal =
            edge1.cross(edge2).normalize();

        normals[face.v0] =
            normals[face.v0] + faceNormal;

        normals[face.v1] =
            normals[face.v1] + faceNormal;

        normals[face.v2] =
            normals[face.v2] + faceNormal;
    }

    for (auto& n : normals)
    {
        n = n.normalize();
    }


    return true;
}