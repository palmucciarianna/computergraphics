#pragma once
#include "Scene.h"
#include "Shader.h"

class Renderer {
public:
    explicit Renderer(const Shader& shader);
    void drawScene(const Scene& scene, const glm::mat4& projection) const;
private:
    const Shader& shader;
};
