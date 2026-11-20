#include "Renderer.h"

Renderer::Renderer(const Shader& shaderProgram) : shader(shaderProgram) {}

void Renderer::drawScene(const Scene& scene, const glm::mat4& projection) const {
    shader.use();
    shader.setMatrix("projection", projection);
    for (const Instance& object : scene.instances()) {
        shader.setMatrix("model", object.modelMatrix);
        scene.geometry(object.geometryIndex).draw();
    }
}
