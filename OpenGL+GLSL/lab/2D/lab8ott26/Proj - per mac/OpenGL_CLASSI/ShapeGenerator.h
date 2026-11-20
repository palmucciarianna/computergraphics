// ============================================================================
// ShapeGenerator.h - GUIDA DIDATTICA
// Interfaccia del generatore geometrico: produce MeshData senza creare risorse GPU.
// ============================================================================

#pragma once

#include "MeshData.h"

// ============================================================================
// ShapeGenerator
// Costruisce le forme sulla CPU. Nessuna chiamata OpenGL in questa classe.
//
// Punto 2: curve parametriche campionate sulla CPU.
// ============================================================================
class ShapeGenerator
{
// PUBLIC: funzioni statiche chiamabili senza creare un oggetto ShapeGenerator.
public:
    // Genera tre posizioni e tre colori sulla CPU e restituisce MeshData.
    // Ogni parametro e' un colore RGBA passato per riferimento costante.
    static MeshData createTriangle(
        const glm::vec4& color1,
        const glm::vec4& color2,
        const glm::vec4& color3
    );
    // center: centro; scale: dimensioni; segments: numero di segmenti.
    // Cerchio, cuore e farfalla: GL_TRIANGLE_FAN. Luna: GL_TRIANGLES.
    static MeshData createCircle(const glm::vec2& center, float radius,
                                int segments, const glm::vec4& color);
    static MeshData createHeart(const glm::vec2& center, float scale,
                               int segments, const glm::vec4& color);
    static MeshData createButterfly(const glm::vec2& center, float scale,
                                   int segments, const glm::vec4& color);
    static MeshData createMoon(const glm::vec2& center, float radius,
                              int segments, const glm::vec4& color);
};
