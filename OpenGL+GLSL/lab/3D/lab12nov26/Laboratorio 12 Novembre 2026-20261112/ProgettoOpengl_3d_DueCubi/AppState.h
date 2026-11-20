#pragma once
#include <glm/glm.hpp>
#include <string>

struct AppState {
    glm::vec4 background{0.12f, 0.16f, 0.22f, 1.0f};
    bool wireframe = false;
    bool showAnchor = true;
    int selected = -1;

    // Parametri Cubo Automatico
    bool autoAnimate = true;
    float autoSpeed = 1.2f;
    float autoRadius = 3.5f;
    float autoHeight = 0.5f;
    glm::vec3 autoPos{0.0f, 0.5f, 0.0f};
    glm::vec3 autoColor{0.95f, 0.45f, 0.15f}; // arancione vivace

    // Parametri Cubo Manuale
    glm::vec3 manualPos{0.0f, 0.5f, 0.0f};
    glm::vec3 manualRot{0.0f, 0.0f, 0.0f}; // gradi su X, Y, Z
    float manualScale = 1.0f;
    glm::vec3 manualColor{0.20f, 0.70f, 0.95f}; // azzurro brillante
};
