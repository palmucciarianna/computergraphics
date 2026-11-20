#include <cmath>
#include <fstream>
#include <string>
#include <cstdio>
#include "strutture.h"
#include "imgui.h"           // Libreria core ImGui
#include "imgui_impl_glfw.h" // Integrazione ImGui con GLFW (gestione finestra/input)
#include "imgui_impl_opengl3.h" // Integrazione ImGui con OpenGL3 (rendering)
#include "lib.h"
#include "gestione_curve.h"
#include "gestione_multicurva.h"
#include "gestione_interazioni.h"
#include "esportazione_obj.h"
#include "inizializzazioni.h"

extern ImVec4 clear_color;  // Colore di sfondo (RGBA), dichiarato esternamente
extern double mousex, mousey; // Coordinate del mouse, dichiarate esternamente
extern bool flag_wireframe;
extern int width, height;
extern int Mod, selected_point;
extern bool visualizzaTg, modTg;
extern bool riempi, visualizza_PC;
extern Curva* curva;
extern float* t;
extern float Tens, Bias, Cont;
extern int param;

// Funzione che inizializza ImGui con GLFW e OpenGL
void Initialize_IMGUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();               // Controlla compatibilità versione ImGui
    ImGui::CreateContext();             // Crea il contesto ImGui (necessario)
    ImGuiIO& io = ImGui::GetIO();       // Ottiene l’oggetto IO di ImGui (config e input)
    io.FontGlobalScale = 1.15f;         // Scala bilanciata e nitida su display Retina
    io.IniFilename = NULL;              // Disabilita cache imgui.ini: la finestra apparirà sempre aperta e ben posizionata
    ImGui::StyleColorsDark();           // Imposta tema scuro predefinito
    
    // Inizializza l’integrazione con GLFW per OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

// Funzione di rendering dell’interfaccia grafica per ogni frame
void my_interface()
{
    ImGui_ImplGlfw_NewFrame();          // Prepara frame nuovo per input GLFW
    ImGui_ImplOpenGL3_NewFrame();       // Prepara frame nuovo per rendering OpenGL3
    ImGui::NewFrame();                  // Inizia registrazione nuova UI frame

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 720), ImGuiCond_FirstUseEver);

    ImGui::Begin("Pannello Modellatore Hermite 2D", NULL, ImGuiWindowFlags_NoCollapse);

    // 1. Gestione dei punti
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.3f, 1.0f), "--- GESTIONE PUNTI ---");
    ImGui::RadioButton("Inserimento", &Mod, 0); ImGui::SameLine();
    ImGui::RadioButton("Modifica", &Mod, 1); ImGui::SameLine();
    if (ImGui::RadioButton("Elimina", &Mod, 2)) {
        modTg = false;
        selected_point = -1;
    }
    if (Mod == 0) selected_point = -1;

    ImGui::Separator();

    // 2. Curve multiple e Colori
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "--- CURVE MULTIPLE (FORME COMPLESSE) ---");
    static int numeroRichiesto = 1;
    ImGui::PushItemWidth(140);
    ImGui::InputInt("Numero curve", &numeroRichiesto);
    ImGui::PopItemWidth();
    if (numeroRichiesto < 1) numeroRichiesto = 1;
    if (numeroRichiesto > 20) numeroRichiesto = 20;
    ImGui::SameLine();
    if (ImGui::Button("Crea curve")) {
        CambiaNumeroCurve(numeroRichiesto);
        Mod = 0;
        modTg = false;
        selected_point = -1;
    }

    ImGui::Text("Disponibili: %d | Curva attiva: %d (Punti: %d)",
        static_cast<int>(curve.size()),
        curvaAttivaIndice + 1,
        curva ? static_cast<int>(curva->CP.size()) : 0);

    for (int k = 0; k < static_cast<int>(curve.size()); ++k) {
        if (k > 0 && k % 4 != 0) ImGui::SameLine();
        char nome[32];
        std::snprintf(nome, sizeof(nome), "Curva %d", k + 1);
        if (ImGui::RadioButton(nome, curvaAttivaIndice == k)) {
            SelezionaCurva(k);
            Mod = 0;
            modTg = false;
            selected_point = -1;
        }
    }
    if (curva) {
        ImGui::ColorEdit3("Colore pezzo attivo", &curva->coloreCurva.r);
    }

    ImGui::Separator();

    // 3. Parametrizzazione, Chiusura e Triangolazione
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "--- GEOMETRIA & CHIUSURA C1 ---");
    ImGui::RadioButton("Uniforme", &param, 0); ImGui::SameLine();
    ImGui::RadioButton("Secondo le corde", &param, 1);
    ImGui::Checkbox("Poligono di Controllo", &visualizza_PC); ImGui::SameLine();
    ImGui::Checkbox("Wireframe", &flag_wireframe);

    if (ImGui::Button("Chiudi Forma (C1)")) {
        ImpostaChiusuraC1(curva);
    }
    if (curva && curva->chiusaC1) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "[C1 Attiva]");
    }
    ImGui::Checkbox("Renderizza (Riempimento pieno)", &riempi);

    ImGui::Separator();

    // 4. Tangenti
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "--- TANGENTI ---");
    ImGui::Checkbox("Visualizza Tangenti", &visualizzaTg); ImGui::SameLine();
    if (ImGui::Checkbox("Modifica Tangenti", &modTg) && modTg) {
        Mod = 1;
    }

    ImGui::Separator();

    // 5. Sfondo
    ImGui::ColorEdit4("Colore Sfondo", (float*)&clear_color);

    ImGui::Separator();

    // 6. Salvataggio ed Esportazione
    ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.2f, 1.0f), "--- SALVATAGGIO ED ESPORTAZIONE ---");
    static char percorsoForma[256] = "myshape.txt";
    static std::string messaggioForma;
    ImGui::InputText("File forma (.txt)", percorsoForma, sizeof(percorsoForma));
    if (ImGui::Button("Salva Forma")) {
        std::string errore;
        if (SalvaForma(percorsoForma, errore)) messaggioForma = "Forma salvata correttamente.";
        else messaggioForma = errore;
    }
    ImGui::SameLine();
    if (ImGui::Button("Apri Forma")) {
        std::string errore;
        if (ApriForma(percorsoForma, errore)) {
            selected_point = -1;
            numeroRichiesto = static_cast<int>(curve.size());
            messaggioForma = "Forma caricata correttamente.";
        } else messaggioForma = errore;
    }
    if (!messaggioForma.empty()) ImGui::TextWrapped("%s", messaggioForma.c_str());

    static char percorsoOBJ[256] = "myshape.obj";
    static std::string messaggioOBJ;
    ImGui::InputText("File OBJ (.obj)", percorsoOBJ, sizeof(percorsoOBJ));
    if (ImGui::Button("Esporta Mesh OBJ/MTL")) {
        EsportaFormaOBJ(percorsoOBJ, messaggioOBJ);
    }
    if (!messaggioOBJ.empty()) ImGui::TextWrapped("%s", messaggioOBJ.c_str());

    ImGui::Separator();
    ImGui::Text("Singola curva (mycurve_N.txt)");
    if (ImGui::Button("Salva Curva")) {
        std::ofstream outputFile(("mycurve_" + std::to_string(curvaAttivaIndice + 1) + ".txt").c_str());
        if (outputFile.is_open()) {
            outputFile << "# C1 " << (curva->chiusaC1 ? 1 : 0) << "\n";
            outputFile << "# RGB " << curva->coloreCurva.r << " " << curva->coloreCurva.g << " " << curva->coloreCurva.b << "\n";
            for (size_t i = 0; i < curva->CP.size(); i++)
                outputFile << curva->CP[i].x << "  " << curva->CP[i].y << "  " << curva->CP[i].z << "  "
                           << curva->Tens[i] << "  " << curva->Bias[i] << "  " << curva->Cont[i] << "\n";
            outputFile.close();
            std::cout << "Dati scritti con successo." << std::endl;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Apri Curva")) {
        FILE* file = std::fopen(("mycurve_" + std::to_string(curvaAttivaIndice + 1) + ".txt").c_str(), "r");
        if (file != NULL) {
            struct Dati { float x, y, z, Tens, Bias, Cont; } dati[1000];
            int riga = 0;
            int closedFromFile = 0;
            char header[128]; long offset = std::ftell(file);
            if (std::fgets(header, sizeof(header), file)) {
                if (std::sscanf(header, "# C1 %d", &closedFromFile) != 1) std::fseek(file, offset, SEEK_SET);
                else {
                    long rgbOffset = std::ftell(file);
                    if (std::fgets(header, sizeof(header), file)) {
                        float r, g, b;
                        if (std::sscanf(header, "# RGB %f %f %f", &r, &g, &b) == 3 &&
                            std::isfinite(r) && std::isfinite(g) && std::isfinite(b) &&
                            r >= 0.f && r <= 1.f && g >= 0.f && g <= 1.f && b >= 0.f && b <= 1.f)
                            curva->coloreCurva = vec4(r, g, b, 1.f);
                        else std::fseek(file, rgbOffset, SEEK_SET);
                    }
                }
            }
            while (std::fscanf(file, "%f %f %f %f %f %f ", &dati[riga].x, &dati[riga].y, &dati[riga].z,
                               &dati[riga].Tens, &dati[riga].Bias, &dati[riga].Cont) == 6) {
                riga++;
                if (riga >= 1000) break;
            }
            std::fclose(file);
            curva->chiusaC1 = false;
            curva->CP.clear(); curva->colCP.clear(); curva->Tens.clear(); curva->Bias.clear(); curva->Cont.clear(); curva->Derivata.clear();
            for (int i = 0; i < riga; i++) {
                curva->CP.push_back(vec3(dati[i].x, dati[i].y, dati[i].z));
                curva->Tens.push_back(dati[i].Tens);
                curva->Bias.push_back(dati[i].Bias);
                curva->Cont.push_back(dati[i].Cont);
                curva->colCP.push_back(vec4(0.0, 0.0, 0.0, 1.0));
                curva->Derivata.push_back(vec3(0.0, 0.0, 0.0));
            }
            curva->ncp = curva->CP.size();
            curva->chiusaC1 = (closedFromFile != 0 && curva->CP.size() >= 4);
            UPDATE_VAO_DYNAMIC(curva, true);
        }
    }

    { 
        const ImVec2 p = ImGui::GetWindowPos(), z = ImGui::GetWindowSize();
        RegistraAreaPannello(p.x, p.y, z.x, z.y, 0);
        RegistraAreaPannello(0, 0, 0, 0, 1);
    }
    ImGui::End(); // Termina la finestra
    ImGui::Render(); // Renderizza gli elementi dell'interfaccia ImGui
}

void close_GUI(void) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
