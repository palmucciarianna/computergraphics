#pragma once
#include "Geometry.h"
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

// Ogni istanza condivide una geometria e possiede la propria Model Matrix.
struct Instance {
    std::size_t geometryIndex = 0;
    glm::mat4 modelMatrix{1.0f};
};

class Scene {
public:
    std::size_t addGeometry(const MeshData& mesh);
    std::size_t addInstance(std::size_t geometryIndex);
    Instance& instance(std::size_t index);
    const std::vector<Instance>& instances() const;
    const Geometry& geometry(std::size_t index) const;
private:
    std::vector<std::unique_ptr<Geometry>> geometries;
    std::vector<Instance> objects;
};
