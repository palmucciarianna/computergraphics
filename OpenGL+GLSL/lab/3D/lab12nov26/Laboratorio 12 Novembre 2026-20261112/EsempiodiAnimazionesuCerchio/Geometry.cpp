#include "Geometry.h"

Geometry::Geometry(const MeshData& data) {
    count = static_cast<GLsizei>(data.positions.size());
    primitive = data.primitive;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, positionsVbo);
    glBufferData(GL_ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec2), data.positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colorsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
    glBufferData(GL_ARRAY_BUFFER, data.colors.size() * sizeof(glm::vec3), data.colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Geometry::~Geometry() {
    glDeleteBuffers(1, &positionsVbo);
    glDeleteBuffers(1, &colorsVbo);
    glDeleteVertexArrays(1, &vao);
}

void Geometry::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(primitive, 0, count);
    glBindVertexArray(0);
}
