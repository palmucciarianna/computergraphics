#include "Scene.h"

#include <utility>


std::size_t Scene::add(
    const MeshData& data,
    bool dynamic)
{
    Geometry geometry;

    geometry.setData(
        data.positions,
        data.colors,
        data.primitiveType
    );

    geometry.upload(dynamic);

    m_objects.push_back(
        std::move(geometry)
    );

    return m_objects.size() - 1;
}


void Scene::clear()
{
    // I distruttori di Geometry eliminano VAO e VBO.
    m_objects.clear();
}
