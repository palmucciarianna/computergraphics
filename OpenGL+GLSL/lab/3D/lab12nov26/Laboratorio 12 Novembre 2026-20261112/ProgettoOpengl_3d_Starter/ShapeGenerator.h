#pragma once
#include "MeshData.h"
class ShapeGenerator {
public:
    static MeshData cube();
    static MeshData plane();
    static MeshData pyramid();
    static MeshData sphere(int slices = 32, int stacks = 16);
    static MeshData torus(int slices = 36, int rings = 18);
};
