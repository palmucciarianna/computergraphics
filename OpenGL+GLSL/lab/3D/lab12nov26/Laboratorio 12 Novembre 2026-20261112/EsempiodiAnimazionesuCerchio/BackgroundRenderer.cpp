#include "BackgroundRenderer.h"

static MeshData makeFullscreenQuad() {
    MeshData m;
    m.positions = {{-1,-1},{1,-1},{1,1}, {-1,-1},{1,1},{-1,1}};
    m.colors.assign(6, {1,1,1});
    return m;
}
BackgroundRenderer::BackgroundRenderer(const Shader& program)
    : shader(program), quad(makeFullscreenQuad()) {}
void BackgroundRenderer::draw(float time) const {
    shader.use();
    shader.setFloat("time", time);
    quad.draw();
}
