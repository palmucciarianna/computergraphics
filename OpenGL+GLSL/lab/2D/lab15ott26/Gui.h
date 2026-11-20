#pragma once

#include <glm/glm.hpp>

// Dichiarazione anticipata del tipo GLFWwindow.
// Evita di includere GLFW direttamente nell'header.
struct GLFWwindow;

// ============================================================================
// Gui
// ----------------------------------------------------------------------------
// Incapsula Dear ImGui in una classe.
//
// Responsabilita':
//   - inizializzare ImGui;
//   - iniziare un nuovo frame dell'interfaccia;
//   - costruire i widget;
//   - disegnare l'interfaccia;
//   - conservare i parametri scelti dall'utente.
//
// IMPORTANTE:
// Gui NON esegue il rendering della scena OpenGL.
// Fornisce semplicemente al Renderer i parametri scelti dall'utente.
// ============================================================================

class Gui
{
public:

    // Inizializza Dear ImGui e i backend GLFW/OpenGL3.
    void initialize(
        GLFWwindow* window
    );


    // Inizia un nuovo frame ImGui.
    void beginFrame();


    // Costruisce i controlli visibili nell'interfaccia.
    void draw();


    // Disegna l'interfaccia sopra la scena OpenGL.
    void render();


    // Libera le risorse usate da Dear ImGui.
    // Deve essere chiamato PRIMA di distruggere la finestra GLFW.
    void shutdown();


    // Restituisce il colore scelto per lo sfondo.
    const glm::vec4& clearColor() const
    {
        return m_clearColor;
    }

    bool wireframe() const
    {
        return m_wireframe;
    }

    int numRows() const {
        return  m_numRows;
    }

    int numCols() const {
        return  m_numCols;
    }


private:

    // Colore iniziale dello sfondo.
    // E' lo stesso colore che il progetto usava prima in Renderer::beginFrame().
    glm::vec4 m_clearColor =
        glm::vec4(
            0.0f,
            0.2f,
            1.0f,
            1.0f
        );

    bool m_initialized = false;
    bool m_wireframe = false;
    int  m_numRows = 5;
    int m_numCols = 5;
};
