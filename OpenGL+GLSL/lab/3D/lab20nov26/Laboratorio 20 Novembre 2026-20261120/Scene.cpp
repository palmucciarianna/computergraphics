#include "Scene.h"
#include <stdexcept>
std::size_t Scene::addGeometry(const MeshData& data) {
    geometries_.push_back(std::make_unique<Geometry>(data));
    return geometries_.size()-1;
}
std::size_t Scene::addInstance(std::size_t geometryIndex, const std::string& name) {
    if (geometryIndex >= geometries_.size()) throw std::out_of_range("Geometria inesistente");
    instances_.push_back({geometryIndex, name});
    return instances_.size()-1;
}
void Scene::removeInstance(std::size_t index) {
    if (index < instances_.size()) instances_.erase(instances_.begin()+index);
}


