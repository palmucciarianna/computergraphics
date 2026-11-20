#include "Scene.h"
#include <stdexcept>

std::size_t Scene::addGeometry(const MeshData& mesh) {
    geometries.push_back(std::make_unique<Geometry>(mesh));
    return geometries.size() - 1;
}
std::size_t Scene::addInstance(std::size_t geometryIndex) {
    if (geometryIndex >= geometries.size()) throw std::out_of_range("Geometria inesistente");
    objects.push_back(Instance{geometryIndex, glm::mat4(1.0f)});
    return objects.size() - 1;
}
Instance& Scene::instance(std::size_t index) { return objects.at(index); }
const std::vector<Instance>& Scene::instances() const { return objects; }
const Geometry& Scene::geometry(std::size_t index) const { return *geometries.at(index); }
