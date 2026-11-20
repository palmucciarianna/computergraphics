#pragma once
#include "MeshData.h"

// Una geometria possiede un VAO e due VBO.
class Geometry {
public:
    explicit Geometry(const MeshData& data);
    ~Geometry();
    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;
    void draw() const;
private:
    GLuint vao = 0;
    GLuint positionsVbo = 0;
    GLuint colorsVbo = 0;
    GLsizei count = 0;
    GLenum primitive = GL_TRIANGLES;
};
