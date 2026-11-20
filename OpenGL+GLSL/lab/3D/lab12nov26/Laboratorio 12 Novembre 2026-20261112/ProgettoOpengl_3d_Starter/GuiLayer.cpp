#include "GuiLayer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
 
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
 

GuiLayer::GuiLayer(GLFWwindow* window,Scene& scene,AppState& state,
                   const std::array<std::size_t,5>& geometryIds)
 :scene_(scene),state_(state),geometryIds_(geometryIds) {
 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
 
}
GuiLayer::~GuiLayer() {
 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
 
}
void GuiLayer::addObject(int type) {
    static const char* names[]={"Cubo","Piano","Piramide","Sfera","Toro"};
    if(type<0||type>=5) return;
    std::size_t id=scene_.addInstance(geometryIds_[type],
                       std::string(names[type])+" "+std::to_string(++counters_[type]));
    // Anche le nuove istanze usano la stessa geometria gia' caricata nella GPU.
    glm::mat4 model(1.0f);
    model=glm::translate(model,{0.0f,1.0f,0.0f});
    scene_.instance(id).modelMatrix=model;
    scene_.instance(id).selectable=true;
    state_.selected=static_cast<int>(id);
}
void GuiLayer::beginFrame() {
 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
 
}
void GuiLayer::draw() {
 
    ImGui::Begin("Scena 3D - controlli");
    ImGui::ColorEdit4("Sfondo",&state_.background.x);
    ImGui::Checkbox("Wireframe",&state_.wireframe);
    ImGui::Checkbox("Mostra ancora",&state_.showAnchor);
    ImGui::Checkbox("Navigazione (mouse)",&state_.navigationMode);
    ImGui::Separator();
    const char* names[]={"Cubo","Piano","Piramide","Sfera","Toro"};
    ImGui::Combo("Nuova primitiva",&type_,names,5);
    if(ImGui::Button("Aggiungi oggetto")) addObject(type_);
    ImGui::Separator();
    // Selezione semplice e stabile: si puo' scegliere un oggetto dalla lista.
    // La selezione cambia SOLO quando si clicca su un nome in questa lista.
    ImGui::Text("Oggetti nella scena:");
    for (int i = 0; i < int(scene_.instances().size()); ++i) {
        const Instance& object = scene_.instance(i);
        if (!object.selectable) continue;
        if (ImGui::Selectable(object.name.c_str(), state_.selected == i))
            state_.selected = i;
    }
    ImGui::Separator();
    if(state_.selected>=0 && static_cast<std::size_t>(state_.selected)<scene_.instances().size()) {
        Instance& selected=scene_.instance(state_.selected);
        ImGui::Checkbox("Visibile",&selected.visible);
        // Il colore appartiene all'istanza, non alla geometria condivisa.
        if (ImGui::ColorEdit3("Colore oggetto", &selected.color.x,
                              ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB)) {
            selected.useUniformColor = true;
        }
        ImGui::Checkbox("Usa colore uniforme", &selected.useUniformColor);
        ImGui::TextDisabled("Disattiva per vedere i colori originali dei vertici.");
        glm::vec3 position(selected.modelMatrix[3]);
        if(ImGui::DragFloat3("Posizione",&position.x,0.1f))
            selected.modelMatrix[3]=glm::vec4(position,1.0f);
        if(ImGui::Button("Elimina selezionato")) {
            scene_.removeInstance(state_.selected);
            state_.selected=-1;
        }
    }
    ImGui::Text("Operazione: %s",state_.operation.c_str());
    ImGui::Text("Asse: %s",state_.axis.c_str());
    ImGui::Separator();
    ImGui::TextWrapped("Seleziona dalla lista | TAB: navigazione ON/OFF | Trascina: orbita camera (solo navigazione)");
    ImGui::TextWrapped("1/2/3: asse X/Y/Z | T: trasla | R: ruota | G: scala | Canc: elimina");
    ImGui::TextWrapped("WASD/QE: muovi camera | Frecce: orbita | Spazio: wireframe");
    ImGui::End();
 
}
void GuiLayer::render() {
 
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
 
}

