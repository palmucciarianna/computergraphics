#include "Renderer.h"
#include <glad/glad.h>
void Renderer::draw(const Scene& scene,const Camera& camera,float aspect,const AppState& state) const {
    shader_.use();
    shader_.setMatrix("uView",camera.view());
    shader_.setMatrix("uProjection",camera.projection(aspect));
    glPolygonMode(GL_FRONT_AND_BACK,state.wireframe?GL_LINE:GL_FILL);
    for(std::size_t i=0;i<scene.instances().size();++i) {
        const Instance& item=scene.instance(i);
        if(!item.visible) continue;
        shader_.setMatrix("uModel",item.modelMatrix);
        shader_.setVec3("uObjectColor", item.color);
        shader_.setInt("uUseUniformColor", item.useUniformColor ? 1 : 0);
        scene.geometry(item.geometryIndex).draw();
    }
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if(state.showAnchor && state.selected>=0 && static_cast<std::size_t>(state.selected)<scene.instances().size()) {
        glDisable(GL_DEPTH_TEST);
        const Instance& item=scene.instance(state.selected);
        shader_.setMatrix("uModel",item.modelMatrix);
        shader_.setVec3("uObjectColor", glm::vec3(1.0f));
        shader_.setInt("uUseUniformColor", 1);
        scene.geometry(item.geometryIndex).drawAnchor();
        glEnable(GL_DEPTH_TEST);
    }
}
