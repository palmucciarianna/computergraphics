#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Gui.h"

// Variabile definita nel main.cpp.
// La GUI la modifica direttamente attraverso ImGui::ColorEdit4.
extern ImVec4 clear_color;

void Initialize_IMGUI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void my_interface(void)
{
    // Inizio del nuovo frame ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Piccola finestra fissa nell'angolo in alto a sinistra.
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);

    ImGui::Begin(
        "Impostazioni",
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove
    );

    // UNICO controllo dell'interfaccia:
    // modifica il colore usato da glClearColor nel main.cpp.
    ImGui::ColorEdit4("Colore sfondo", (float*)&clear_color);

    ImGui::End();

    // Genera i dati di rendering della GUI.
    ImGui::Render();
}

void close_GUI(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
