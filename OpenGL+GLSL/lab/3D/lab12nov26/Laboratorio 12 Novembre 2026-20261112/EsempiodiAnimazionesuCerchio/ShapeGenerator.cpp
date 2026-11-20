#include "ShapeGenerator.h"
#include <cmath>

MeshData ShapeGenerator::triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 color) {
    MeshData m;
    m.positions = {a, b, c};
    m.colors = {color, color, color};
    return m;
}

MeshData ShapeGenerator::rectangle(float width, float height, glm::vec3 color) {
    float x = width / 2.0f;
    float y = height / 2.0f;
    MeshData m;
    m.positions = {{-x,-y},{x,-y},{x,y}, {-x,-y},{x,y},{-x,y}};
    m.colors.assign(m.positions.size(), color);
    return m;
}

MeshData ShapeGenerator::circle(float radius, int segments, glm::vec3 color) {
    MeshData m;
    const float pi = 3.14159265358979323846f;
    for (int i = 0; i < segments; ++i) {
        float a = 2.0f * pi * i / segments;
        float b = 2.0f * pi * (i + 1) / segments;
        m.positions.push_back({0.0f, 0.0f});
        m.positions.push_back({radius * std::cos(a), radius * std::sin(a)});
        m.positions.push_back({radius * std::cos(b), radius * std::sin(b)});
    }
    m.colors.assign(m.positions.size(), color);
    return m;
}

MeshData ShapeGenerator::ring(float radius, int segments, glm::vec3 color) {
    MeshData m;
    m.primitive = GL_LINE_LOOP;
    const float pi = 3.14159265358979323846f;
    for (int i = 0; i < segments; ++i) {
        float a = 2.0f * pi * i / segments;
        m.positions.push_back({radius * std::cos(a), radius * std::sin(a)});
    }
    m.colors.assign(m.positions.size(), color);
    return m;
}
