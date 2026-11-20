#pragma once
#include "MeshData.h"

class ShapeGenerator {
public:
    static MeshData rectangle(float width, float height, glm::vec3 color);
    static MeshData triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 color);
    static MeshData circle(float radius, int segments, glm::vec3 color);
    static MeshData ring(float radius, int segments, glm::vec3 color);
};
