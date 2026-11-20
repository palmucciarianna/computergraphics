#include "Geometry.h"
#include <cstddef>
Geometry::Geometry(const MeshData& data) {
    indexCount_=static_cast<GLsizei>(data.indices.size());
    glGenVertexArrays(1,&vao_); glBindVertexArray(vao_);
    glGenBuffers(1,&vbo_); glBindBuffer(GL_ARRAY_BUFFER,vbo_);
    glBufferData(GL_ARRAY_BUFFER,data.vertices.size()*sizeof(Vertex),data.vertices.data(),GL_STATIC_DRAW);
    glGenBuffers(1,&ebo_); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,data.indices.size()*sizeof(unsigned),data.indices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
Geometry::~Geometry() { glDeleteBuffers(1,&ebo_);glDeleteBuffers(1,&vbo_);glDeleteVertexArrays(1,&vao_); }
void Geometry::draw() const { glBindVertexArray(vao_);glDrawElements(GL_TRIANGLES,indexCount_,GL_UNSIGNED_INT,nullptr);glBindVertexArray(0); }

void Geometry::drawAnchor() const {
    // Punto di riferimento locale: un vertice della mesh, non il centro geometrico.
    if (indexCount_==0) return;
    glBindVertexArray(vao_);
    glPointSize(15.0f);
    glDrawElements(GL_POINTS,1,GL_UNSIGNED_INT,nullptr);
    glBindVertexArray(0);
}
