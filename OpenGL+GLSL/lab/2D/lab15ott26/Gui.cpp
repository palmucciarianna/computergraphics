#include "Gui.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <../ImGui/imgui.h>
#include <../ImGui/imgui_impl_glfw.h>
#include <../ImGui/imgui_impl_opengl3.h>


void Gui::initialize(
    GLFWwindow* window)
{
    if (m_initialized)
        return;

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    // Tema grafico scuro.
    ImGui::StyleColorsDark();


    // true = il backend ImGui installa le proprie callback GLFW.
    // Le callback gia' registrate dal progetto vengono concatenate
    // dal backend ImGui.
    ImGui_ImplGlfw_InitForOpenGL(
        window,
        true
    );


    ImGui_ImplOpenGL3_Init(
        "#version 330 core"
    );

    m_initialized = true;
}


void Gui::beginFrame()
{
    if (!m_initialized)
        return;

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
}


void Gui::draw()
{
    ImGui::Begin(
        "Controlli"
    );


    // ------------------------------------------------------------------------
    // COLORE DELLO SFONDO
    // ------------------------------------------------------------------------

    ImGui::Text(
        "Colore dello sfondo"
    );

    ImGui::ColorEdit3(
        "Background",
        &m_clearColor.r
    );


    ImGui::Separator();


    // ------------------------------------------------------------------------
    // MODALITA' DI RASTERIZZAZIONE
    // ------------------------------------------------------------------------

    ImGui::Text(
        "Modalita' di visualizzazione"
    );


    ImGui::Checkbox(
        "Wireframe",
        &m_wireframe
    );

    ImGui::Text("Griglia di oggetti");
    ImGui::SliderInt("Righe", &m_numRows, 1, 8);
    ImGui::SliderInt("Colonne", &m_numCols, 1, 8);
    ImGui::End();
}


void Gui::render()
{
    if (!m_initialized)
        return;

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData()
    );
}


void Gui::shutdown()
{
    if (!m_initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    m_initialized = false;
}
