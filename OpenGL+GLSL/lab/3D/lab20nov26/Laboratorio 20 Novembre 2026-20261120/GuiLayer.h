#pragma once
#include "Scene.h"
#include <glm/glm.hpp>
#include <array>
#include <string>
#include <cstddef>
struct GLFWwindow;

// La GUI possiede le impostazioni 
class GuiLayer {
public:
    GuiLayer(GLFWwindow* window, Scene& scene,
             const std::array<std::size_t, 5>& geometryIds);
    ~GuiLayer();
    void beginFrame();
    void draw();
    void render();

    const glm::vec4& background() const { return background_; }
    bool wireframe() const { return wireframe_; }
    bool showAnchor() const { return showAnchor_; }
    bool navigationMode() const { return navigationMode_; }
    int selected() const { return selected_; }
    const std::string& operation() const { return operation_; }
    const std::string& axis() const { return axis_; }

    // Input da tastiera: non si accede direttamente ai membri privati.
    void toggleWireframe() { wireframe_ = !wireframe_; }
    void toggleNavigationMode() { navigationMode_ = !navigationMode_; }
    void setSelected(int index) { selected_ = index; }
    void setAxis(const std::string& axis) { axis_ = axis; }
    void setOperation(const std::string& operation) { operation_ = operation; }

private:
    Scene& scene_;
    std::array<std::size_t, 5> geometryIds_;
    std::array<int, 5> counters_{};
    int type_ = 0;
    glm::vec4 background_{0.12f, 0.16f, 0.22f, 1.0f};
    bool wireframe_ = false;
    bool showAnchor_ = true;
    bool navigationMode_ = false;
    int selected_ = -1;
    std::string operation_ = "Nessuna";
    std::string axis_ = "Y";

    void drawSettings();
    void drawAddObject();
    void addObject(int type);
    void drawObjectList();
    void drawSelectedObject();
};
