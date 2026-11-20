#pragma once
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "AppState.h"
class Renderer {
public:
    explicit Renderer(const Shader& shader):shader_(shader) {}
    void draw(const Scene& scene,const Camera& camera,float aspect,const AppState& state) const;
private:
    const Shader& shader_;
};
