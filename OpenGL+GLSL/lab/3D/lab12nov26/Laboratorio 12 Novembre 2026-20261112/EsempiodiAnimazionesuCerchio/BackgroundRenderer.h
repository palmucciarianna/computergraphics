#pragma once
#include "Shader.h"
#include "Geometry.h"

// Lo sfondo non usa la Scene: disegna un quad fullscreen in NDC.
class BackgroundRenderer {
public:
    explicit BackgroundRenderer(const Shader& shader);
    void draw(float time) const;
private:
    const Shader& shader;
    Geometry quad;
};
