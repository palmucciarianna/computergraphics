#include "GuiLayer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GuiLayer::GuiLayer(GLFWwindow* window, Scene& scene,
                   const std::array<std::size_t, 5>& geometryIds)
    : scene_(scene), geometryIds_(geometryIds) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}
GuiLayer::~GuiLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void GuiLayer::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void GuiLayer::draw() {
    ImGui::Begin("Scena 3D - controlli");
    drawSettings(); ImGui::Separator();
    drawAddObject(); ImGui::Separator();
    drawObjectList(); ImGui::Separator();
    drawSelectedObject(); ImGui::Separator();
    ImGui::Text("Operazione: %s", operation_.c_str());
    ImGui::Text("Asse: %s", axis_.c_str());
    ImGui::TextWrapped("TAB: navigazione | Trascina: orbita camera");
    ImGui::TextWrapped("1/2/3: asse X/Y/Z | T/R/G: trasla/ruota/scala | Canc: elimina");
    ImGui::TextWrapped("WASD/QE: camera | Frecce: orbita | Spazio: wireframe");
    ImGui::End();
}
void GuiLayer::drawSettings() {
    // TODO 1 - Impostazioni generali della scena.
    // Inserire i widget ImGui per modificare i membri privati:
    //   background_      -> ImGui::ColorEdit4
    //   wireframe_       -> ImGui::Checkbox
    //   showAnchor_      -> ImGui::Checkbox
    //   navigationMode_  -> ImGui::Checkbox
    // Suggerimento: il primo float di un glm::vec4 e' &background_.x.
    ImGui::ColorEdit4("Sfondo", &background_.x);
    // GuiLayer background_ is a glm::vec4, so we can pass the address
    ImGui::Checkbox("Wireframe", &wireframe_);
    ImGui::Checkbox("Mostra Ancore", &showAnchor_);
    ImGui::Checkbox("Modalità Navigazione", &navigationMode_);
    // ImGui::TextDisabled("TODO 1: impostazioni della scena");
}

void GuiLayer::drawAddObject() {
    // TODO 2 - Selezione della primitiva e pulsante di creazione.
    // Tipi disponibili: Cubo, Piano, Piramide, Sfera, Toro.
    // 1. Definire i nomi delle cinque primitive.
    // 2. Usare ImGui::Combo per modificare type_.
    // 3. Usare ImGui::Button e chiamare addObject(type_) al click.
    // ImGui::TextDisabled("TODO 2: aggiungere una primitiva");
    const char* primitiveNames[] = { "Cubo", "Piano", "Piramide", "Sfera", "Toro" };
    ImGui::Combo("Nuova Primitiva", &type_, primitiveNames, IM_ARRAYSIZE(primitiveNames));
    if(ImGui::Button("Aggiungi oggetto")) {
        addObject(type_);
    }
    // ImGui::TextDisabled("Todo 2: aggiungere una primitiva");
}

void GuiLayer::addObject(int type) {
    // TODO 3 - Creare una nuova ISTANZA senza duplicare la geometria.
    // 1. Controllare che type sia compreso nell'intervallo [0, 4].
    if(type < 0 || type >= static_cast<int>(geometryIds_.size())) {
        return; // Tipo non valido, esci dalla funzione
    }
    // 2. Costruire un nome progressivo con counters_[type].
    const char* names[] = { "Cubo", "Piano", "Piramide", "Sfera", "Toro" };
    std::string name = std::string(names[type]) + " " + std::to_string(++counters_[type]);
    // 3. Usare scene_.addInstance(geometryIds_[type], name).
    std::size_t geometryId = scene_.addInstance(geometryIds_[type], name);
    // 4. Accedere alla nuova Instance, assegnare position=(0,1,0),
    Instance& newInstanceObj = scene_.instance(geometryId);
    //    aggiornare la Model Matrix e impostare selectable=true.
    newInstanceObj.position = glm::vec3(0.0f, 1.0f, 0.0f);
    newInstanceObj.updateModelMatrix();
    newInstanceObj.selectable = true;
    // 5. Memorizzare l'indice della nuova istanza in selected_.
    selected_ = static_cast<int>(geometryId);
}

void GuiLayer::drawObjectList() {
    // TODO 4 - Elenco e selezione delle istanze della scena.
    // 1. Scorrere scene_.instances() utilizzando gli indici.
    ImGui::Text("Elenco delle istanze selezionabili:");
    for(int i=0; i<scene_.instances().size(); i++){
        const Instance& instance = scene_.instances()[i];
    // 2. Ignorare le istanze con selectable == false.
        if(instance.selectable){
    // 3. Usare ImGui::PushID / ImGui::Selectable / ImGui::PopID.
            ImGui::PushID(i);
            bool isSelected = (selected_ == i);
            if(ImGui::Selectable(instance.name.c_str(), isSelected)) {
    // 4. Quando si clicca un nome, aggiornare selected_.
                selected_ = i;
            }
            ImGui::PopID();
        }
    }
    // ImGui::TextDisabled("TODO 4: elenco delle istanze selezionabili");
}

void GuiLayer::drawSelectedObject() {
    // TODO 5 - Proprietà dell'istanza selezionata.
    // 1. Verificare che selected_ sia un indice valido; altrimenti
    //    mostrare "Nessun oggetto selezionato" e terminare il metodo.
    if (selected_ < 0 || selected_ >= static_cast<int>(scene_.instances().size())) {
        ImGui::Text("Nessun oggetto selezionato");
        return; // Nessun oggetto selezionato, termina il metodo
    }
    // 2. Ottenere Instance& mediante scene_.instance(selected_).
    Instance& selectedInstance = scene_.instance(selected_);
    ImGui::Text("Proprietà dell'oggetto selezionato:");
    ImGui::Text("Nome: %s", selectedInstance.name.c_str());
    // 3. Aggiungere controlli per visible, color, useUniformColor.
    if (ImGui::ColorEdit3("Colore", &selectedInstance.color.x)) {
        selectedInstance.useUniformColor = true; // se si clicca il pulsante colore, impostare useUniformColor a true
    }
    ImGui::Checkbox("Visibile", &selectedInstance.visible);
    ImGui::Checkbox("Usa colore uniforme", &selectedInstance.useUniformColor);
    // 4. Usare ImGui::DragFloat3 per position, rotation e scale.
    bool p = ImGui::DragFloat3("Posizione", &selectedInstance.position.x, 0.1f);
    bool r = ImGui::DragFloat3("Rotazione", &selectedInstance.rotation.x, 1.0f);
    bool s = ImGui::DragFloat3("Scala", &selectedInstance.scale.x, 0.05f);
    if (s) {
        selectedInstance.scale = glm::max(selectedInstance.scale, glm::vec3(0.01f));
    }
    // 5. Se cambiano i parametri TRS, chiamare updateModelMatrix().
    if (p || r || s) {
        selectedInstance.updateModelMatrix();
    }
    // 6. Aggiungere un pulsante per eliminare l'istanza e riportare
    //    selected_ a -1 (nessuna selezione).
    if (ImGui::Button("Elimina oggetto")) {
        scene_.removeInstance(selected_);
        selected_ = -1;
    }
    // Suggerimento: mantenere scale >= 0.01 per ciascun asse.
    // ImGui::TextDisabled("TODO 5: proprieta' dell'oggetto selezionato");
}

void GuiLayer::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
