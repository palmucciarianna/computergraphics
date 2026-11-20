#pragma once
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
class Renderer {
public:
    explicit Renderer(const Shader& shader):shader_(shader) {}
    void draw(const Scene& scene,const Camera& camera,float aspect,bool wireframe, bool showAnchor, int selected) const;
private:
    const Shader& shader_;
};
