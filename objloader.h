

#ifndef OBJLOADER_H
#define OBJLOADER_H

#pragma once

#include <vector>
#include <string>

#include "math.h"

struct Face
{
    int v0;
    int v1;
    int v2;
};

class OBJLoader
{
public:

    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;

    std::vector<Face> faces;

    bool load(const std::string& filename);
};


#endif //OBJLOADER_H
