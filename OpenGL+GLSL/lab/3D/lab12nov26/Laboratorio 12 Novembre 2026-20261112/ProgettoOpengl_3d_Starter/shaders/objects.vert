#version 330 core

// Attributi dei vertici (configurati in Geometry.cpp).
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

// Matrici inviate dal Renderer.
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec4 vertexColor;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    vertexColor = aColor;
}
