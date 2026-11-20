#include "GuiLayer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GuiLayer::GuiLayer(GLFWwindow* window, Scene& scene, AppState& state, Camera& camera)
    : scene_(scene), state_(state), camera_(camera)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Stile elegante
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(8.0f, 6.0f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

GuiLayer::~GuiLayer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiLayer::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiLayer::draw()
{
    ImGui::SetNextWindowSize(ImVec2(380, 520), ImGuiCond_FirstUseEver);
    ImGui::Begin("Pannello di Controllo - 3D Due Cubi");

    if (ImGui::CollapsingHeader("1. Cubo Manuale (Controllo a mano)", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "Modifica la posizione o usa i tasti I/K/J/L/U/O:");
        ImGui::DragFloat3("Posizione (X,Y,Z)", &state_.manualPos.x, 0.05f, -15.0f, 15.0f, "%.2f");
        ImGui::DragFloat3("Rotazione (Gradi)", &state_.manualRot.x, 1.0f, -180.0f, 180.0f, "%.1f°");
        ImGui::SliderFloat("Scala", &state_.manualScale, 0.2f, 3.0f, "%.2f");
        ImGui::ColorEdit3("Colore Cubo Manuale", &state_.manualColor.x);
        if (ImGui::Button("Reset Posizione Manuale")) {
            state_.manualPos = glm::vec3(0.0f, 0.5f, 0.0f);
            state_.manualRot = glm::vec3(0.0f);
            state_.manualScale = 1.0f;
        }
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("2. Cubo Automatico (In movimento)", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Animazione attiva (Play/Pause)", &state_.autoAnimate);
        ImGui::SliderFloat("Velocità moto", &state_.autoSpeed, 0.0f, 5.0f, "%.2f");
        ImGui::SliderFloat("Raggio traiettoria", &state_.autoRadius, 0.5f, 8.0f, "%.2f");
        ImGui::SliderFloat("Altezza base", &state_.autoHeight, 0.2f, 5.0f, "%.2f");
        ImGui::ColorEdit3("Colore Cubo Automatico", &state_.autoColor.x);
        ImGui::Text("Coordinate attuali: (%.2f, %.2f, %.2f)",
                    state_.autoPos.x, state_.autoPos.y, state_.autoPos.z);
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("3. Telecamera 3D", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Posizione Camera: (%.2f, %.2f, %.2f)",
                    camera_.position.x, camera_.position.y, camera_.position.z);
        ImGui::Text("Target: (%.2f, %.2f, %.2f)",
                    camera_.target.x, camera_.target.y, camera_.target.z);
        if (ImGui::Button("Reset Visuale Telecamera")) {
            camera_.position = glm::vec3(0.0f, 4.0f, 18.0f);
            camera_.target = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        ImGui::TextDisabled("Controlli Camera:\n"
                            "- W/S: Avanti / Indietro\n"
                            "- A/D: Sinistra / Destra\n"
                            "- Q/E: Alto / Basso\n"
                            "- Frecce o Trascinamento Mouse: Orbita\n"
                            "- Rotella Mouse o Tasti +/-: Zoom");
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("4. Scena & Grafica")) {
        ImGui::ColorEdit4("Colore di Sfondo", &state_.background.x);
        ImGui::Checkbox("Modalità Wireframe (Linee)", &state_.wireframe);
    }

    ImGui::End();
}

void GuiLayer::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
