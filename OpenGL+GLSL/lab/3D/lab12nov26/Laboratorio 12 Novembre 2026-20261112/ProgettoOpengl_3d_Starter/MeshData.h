#pragma once
#include <vector>
#include <glm/glm.hpp>
struct Vertex { glm::vec3 position; glm::vec4 color; };
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
