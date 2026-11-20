#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Dati geometrici sul lato CPU: ogni vertice ha un colore.
struct MeshData {
    std::vector<glm::vec2> positions;
    std::vector<glm::vec3> colors;
    GLenum primitive = GL_TRIANGLES;
};
