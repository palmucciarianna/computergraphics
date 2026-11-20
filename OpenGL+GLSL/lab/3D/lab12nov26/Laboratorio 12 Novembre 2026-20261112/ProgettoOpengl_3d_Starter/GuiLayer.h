#pragma once
#include "Scene.h"
#include "AppState.h"
#include <array>
struct GLFWwindow;

// Pannello ImGui opzionale. Il progetto funziona anche senza ImGui.
class GuiLayer {
public:
    GuiLayer(GLFWwindow* window,Scene& scene,AppState& state,
             const std::array<std::size_t,5>& geometryIds);
    ~GuiLayer();
    void beginFrame();
    void draw();
    void render();
private:
    Scene& scene_;
    AppState& state_;
    std::array<std::size_t,5> geometryIds_;
    std::array<int,5> counters_{};
    int type_=0;
    void addObject(int type);
};
