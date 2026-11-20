// ============================================================================
// ShapeGenerator.cpp - GUIDA DIDATTICA
// Costruzione del triangolo sulla CPU: associa una posizione e un colore a ogni vertice.
// ============================================================================

#include "ShapeGenerator.h"

// La funzione restituisce per valore un MeshData. I tre vertici sono in coordinate locali e hanno colori corrispondenti.
MeshData ShapeGenerator::createTriangle(
    const glm::vec4& color1,
    const glm::vec4& color2,
    const glm::vec4& color3)
{
    MeshData data;

    // Inseriamo i vertici uno alla volta: push_back aggiunge un elemento
    // alla fine del vector delle posizioni.
    // P0: vertice inferiore sinistro.
    data.positions.push_back(glm::vec3(-0.6f, -0.5f, 0.0f));

    // P1: vertice inferiore destro.
    data.positions.push_back(glm::vec3( 0.6f, -0.5f, 0.0f));

    // P2: vertice superiore.
    data.positions.push_back(glm::vec3( 0.0f,  0.6f, 0.0f));

    // Ogni colore ha lo stesso indice del vertice corrispondente.
    data.colors.push_back(color1);  // Colore del vertice P0.
    data.colors.push_back(color2);  // Colore del vertice P1.
    data.colors.push_back(color3);  // Colore del vertice P2.

    data.primitiveType = GL_TRIANGLES;

    return data;
}

#include <cmath>
#include <stdexcept>

namespace
{
    constexpr float PI = 3.14159265358979323846f;

    // evaluate(u) calcola un punto locale, con u tra 0 e 1.
    // La stessa procedura di campionamento viene riutilizzata per tutte le forme.
    template<typename Curve>
    MeshData sampleOutline(const glm::vec2& center, float scale, int segments,
                           const glm::vec4& color, Curve evaluate)
    {
        if (segments < 3)
            throw std::invalid_argument("Servono almeno 3 segmenti");
        if (!std::isfinite(scale) || scale <= 0.0f)
            throw std::invalid_argument("Raggio/scala deve essere positivo e finito");

        MeshData data;
        data.primitiveType = GL_TRIANGLE_FAN;
        // reserve prenota capacita', ma non aggiunge elementi.
        data.positions.reserve(static_cast<std::size_t>(segments) + 2);
        data.colors.reserve(static_cast<std::size_t>(segments) + 2);

        // Centro del ventaglio, seguito dai punti ordinati del bordo.
        data.positions.push_back(glm::vec3(center.x, center.y, 0.0f));
        data.colors.push_back(color);
        for (int i = 0; i < segments; ++i)
        {
            const float u = static_cast<float>(i) / segments;
            const glm::vec2 point = evaluate(u);
            data.positions.push_back(glm::vec3(
                center.x + scale * point.x,
                center.y + scale * point.y, 0.0f));
            data.colors.push_back(color);
        }
        // Ripetiamo il primo punto del bordo per chiudere il ventaglio.
        data.positions.push_back(data.positions[1]);
        data.colors.push_back(color);
        return data;
    }
}

MeshData ShapeGenerator::createCircle(const glm::vec2& center, float radius,
                                     int segments, const glm::vec4& color)
{
    return sampleOutline(center, radius, segments, color, [](float u) {
        const float t = 2.0f * PI * u;
        return glm::vec2(std::cos(t), std::sin(t));
    });
}

MeshData ShapeGenerator::createHeart(const glm::vec2& center, float scale,
                                    int segments, const glm::vec4& color)
{
    return sampleOutline(center, scale, segments, color, [](float u) {
        const float t = 2.0f * PI * u;
        const float sine = std::sin(t);
        const float x = 16.0f * sine * sine * sine;
        const float y = 13.0f * std::cos(t) - 5.0f * std::cos(2.0f * t)
                      - 2.0f * std::cos(3.0f * t) - std::cos(4.0f * t);
        return glm::vec2(x / 18.0f, y / 18.0f);
    });
}

MeshData ShapeGenerator::createButterfly(const glm::vec2& center, float scale,
                                        int segments, const glm::vec4& color)
{
    return sampleOutline(center, scale, segments, color, [](float u) {
        const float t = 2.0f * PI * u;
        // Variante periodica a quattro lobi, chiusa nell'intervallo [0, 2 PI].
        const float r = std::exp(std::cos(t)) - 2.0f * std::cos(4.0f * t);
        return glm::vec2(std::sin(t) * r / 5.0f, std::cos(t) * r / 5.0f);
    });
}

MeshData ShapeGenerator::createMoon(const glm::vec2& center, float radius,
                                   int segments, const glm::vec4& color)
{
    if (segments < 3 || !std::isfinite(radius) || radius <= 0.0f)
        throw std::invalid_argument("Segmenti >= 3 e raggio positivo richiesti");
    MeshData data;
    data.primitiveType = GL_TRIANGLES;
    data.positions.reserve(static_cast<std::size_t>(segments) * 6);
    data.colors.reserve(static_cast<std::size_t>(segments) * 6);
    auto point = [&](float t, float width) {
        return glm::vec3(center.x + radius * width * std::cos(t),
                         center.y + radius * std::sin(t), 0.0f);
    };
    auto add = [&](const glm::vec3& v) {
        data.positions.push_back(v);
        data.colors.push_back(color);
    };
    // Ogni fascia tra arco esterno e interno viene divisa in due triangoli.
    // Un fan unico riempirebbe anche parte della concavita'.
    for (int i = 0; i < segments; ++i)
    {
        const float t0 = PI / 2.0f + PI * static_cast<float>(i) / segments;
        const float t1 = PI / 2.0f + PI * static_cast<float>(i + 1) / segments;
        const auto outer0 = point(t0, 1.0f);
        const auto outer1 = point(t1, 1.0f);
        const auto inner0 = point(t0, 0.45f);
        const auto inner1 = point(t1, 0.45f);
        add(outer0); add(outer1); add(inner1);
        add(outer0); add(inner1); add(inner0);
    }
    return data;
}
