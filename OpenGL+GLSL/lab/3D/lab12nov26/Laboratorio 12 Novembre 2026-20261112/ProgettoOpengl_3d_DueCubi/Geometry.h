#pragma once
#include "MeshData.h"
#include <glad/glad.h>
class Geometry {
public:
    explicit Geometry(const MeshData& data);
    ~Geometry();
    Geometry(const Geometry&)=delete;
    Geometry& operator=(const Geometry&)=delete;
    void draw() const;
    void drawAnchor() const;
private:
    GLuint vao_=0,vbo_=0,ebo_=0;
    GLsizei indexCount_=0;
};
