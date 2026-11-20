#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

// GEOMETRIA: forma condivisibile. ISTANZA: oggetto concreto nella scena.
struct Instance {
    std::size_t geometryIndex = 0;
    std::string name;
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f}; // Colore uniforme scelto nella GUI
    bool useUniformColor = false; // All'inizio conserva i colori dei vertici
    bool visible = true;
    bool selectable = true;
    float pickingRadius = 1.7f; // sfera approssimata nel riferimento locale
};

class Scene {
public:
    std::size_t addGeometry(const MeshData& data);
    std::size_t addInstance(std::size_t geometryIndex, const std::string& name);
    Instance& instance(std::size_t index) { return instances_.at(index); }
    const Instance& instance(std::size_t index) const { return instances_.at(index); }
    const std::vector<Instance>& instances() const { return instances_; }
    const Geometry& geometry(std::size_t index) const { return *geometries_.at(index); }
    void removeInstance(std::size_t index);
private:
    std::vector<std::unique_ptr<Geometry>> geometries_;
    std::vector<Instance> instances_;
};
