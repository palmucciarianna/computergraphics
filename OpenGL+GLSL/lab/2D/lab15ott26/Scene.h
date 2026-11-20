#pragma once

#include "Geometry.h"
#include "ShapeGenerator.h"

#include <vector>

// ============================================================================
// Scene
// ----------------------------------------------------------------------------
// La scena contiene soltanto gli oggetti geometrici.
//
// IMPORTANTE:
// Scene NON esegue il rendering.
// Il rendering viene affidato alla classe Renderer.
//
// In questo modo separiamo:
//
//      Scene      -> cosa contiene la scena
//      Geometry   -> dati + VAO/VBO + draw()
//      Renderer   -> come viene disegnata la scena
// ============================================================================

class Scene
{
public:

    std::size_t add(
        const MeshData& data,
        bool dynamic = false
    );


    Geometry& operator[](std::size_t index)
    {
        return m_objects[index];
    }


    const Geometry& operator[](std::size_t index) const
    {
        return m_objects[index];
    }


    std::size_t size() const
    {
        return m_objects.size();
    }


    // Permette al Renderer di leggere le geometrie della scena.
    const std::vector<Geometry>& objects() const
    {
        return m_objects;
    }


    void clear();


private:

    std::vector<Geometry> m_objects;
};
