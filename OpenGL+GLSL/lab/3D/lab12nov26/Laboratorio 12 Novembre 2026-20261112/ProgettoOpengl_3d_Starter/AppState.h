#pragma once
#include <glm/glm.hpp>
#include <string>

// Stato condiviso tramite riferimenti espliciti: niente variabili globali extern.
struct AppState {
    glm::vec4 background{0.12f, 0.16f, 0.22f, 1.0f};
    bool wireframe = false;
    bool showAnchor = true;
    bool navigationMode = false; // false = interazione con gli oggetti
    int selected = -1;
    std::string operation = "Nessuna";
    std::string axis = "Y";
};
