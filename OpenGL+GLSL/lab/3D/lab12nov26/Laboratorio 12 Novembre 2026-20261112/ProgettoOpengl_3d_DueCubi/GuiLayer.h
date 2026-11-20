#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "AppState.h"
#include "Camera.h"

class GuiLayer {
public:
    GuiLayer(GLFWwindow* window, Scene& scene, AppState& state, Camera& camera);
    ~GuiLayer();

    void beginFrame();
    void draw();
    void render();

private:
    Scene& scene_;
    AppState& state_;
    Camera& camera_;
};
