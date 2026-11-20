#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <imgui.h>

#include "ShapeGenerator.h"
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "AppState.h"
#include "GuiLayer.h"
#include "Input.h"

int main() {
    // ------------------------------------------------------------
    // 1. INIZIALIZZAZIONE DI GLFW E CREAZIONE FINESTRA
    // ------------------------------------------------------------
    if (!glfwInit()) {
        std::cerr << "Errore: Inizializzazione GLFW fallita.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        1200, 800, "OpenGL 3D - Due Cubi (Automatico e Manuale)", nullptr, nullptr);

    if (!window) {
        std::cerr << "Errore: Creazione finestra fallita.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Errore: Inizializzazione GLAD fallita.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Registrazione delle callback per input mouse e tastiera
    glfwSetKeyCallback(window, Input::keyCallback);
    glfwSetCursorPosCallback(window, Input::cursorPositionCallback);
    glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
    glfwSetScrollCallback(window, Input::scrollCallback);
    glfwSetFramebufferSizeCallback(window, Input::framebufferSizeCallback);

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    // Blocco per assicurare che gli oggetti OpenGL siano distrutti prima del contesto GLFW
    {
        // --------------------------------------------------------
        // 2. SHADER, RENDERER, CAMERA, SCENA, STATO E GUI
        // --------------------------------------------------------
        Shader shader("shaders/objects.vert", "shaders/objects.frag");
        Renderer renderer(shader);
        Scene scene;
        Camera camera;
        AppState state;
        GuiLayer gui(window, scene, state, camera);

        // Posizione iniziale camera per una bella vista prospettica d'insieme
        camera.position = glm::vec3(0.0f, 6.0f, 15.0f);
        camera.target   = glm::vec3(0.0f, 1.0f, 0.0f);

        // --------------------------------------------------------
        // 3. CARICAMENTO DELLE FORME (GEOMETRIE CONDIVISE)
        // --------------------------------------------------------
        std::size_t cubeGeometry    = scene.addGeometry(ShapeGenerator::cube());
        std::size_t planeGeometry   = scene.addGeometry(ShapeGenerator::plane());
        std::size_t pyramidGeometry = scene.addGeometry(ShapeGenerator::pyramid());

        // --------------------------------------------------------
        // 4. CREAZIONE DELLE ISTANZE CONCRETE
        // Creiamo DUE CUBI DISTINTI che condividono la stessa geometria cubeGeometry!
        // --------------------------------------------------------

        // Cubo 1: Animato in automatico
        std::size_t autoCube = scene.addInstance(cubeGeometry, "Cubo Automatico");
        scene.instance(autoCube).useUniformColor = true;
        scene.instance(autoCube).color = state.autoColor;

        // Cubo 2: Controllato a mano (da tastiera o da slider GUI)
        std::size_t manualCube = scene.addInstance(cubeGeometry, "Cubo Manuale");
        scene.instance(manualCube).useUniformColor = true;
        scene.instance(manualCube).color = state.manualColor;
        state.manualPos = glm::vec3(-2.0f, 0.5f, 0.0f);

        // Piano di appoggio
        std::size_t plane = scene.addInstance(planeGeometry, "Piano");
        glm::mat4 planeModel(1.0f);
        planeModel = glm::translate(planeModel, glm::vec3(0.0f, -0.05f, 0.0f));
        planeModel = glm::scale(planeModel, glm::vec3(12.0f, 1.0f, 12.0f));
        scene.instance(plane).modelMatrix = planeModel;
        scene.instance(plane).selectable = false;

        // Piramide decorativa al centro dello sfondo
        std::size_t pyramid = scene.addInstance(pyramidGeometry, "Piramide");
        glm::mat4 pyramidModel(1.0f);
        pyramidModel = glm::translate(pyramidModel, glm::vec3(0.0f, 0.0f, -4.5f));
        pyramidModel = glm::scale(pyramidModel, glm::vec3(1.4f));
        scene.instance(pyramid).modelMatrix = pyramidModel;
        scene.instance(pyramid).useUniformColor = false; // conserva colori sfumati dei vertici

        // Gestione del tempo (delta time per animazioni fluide)
        float lastFrame = static_cast<float>(glfwGetTime());
        float animAngle = 0.0f;

        // --------------------------------------------------------
        // 5. CICLO PRINCIPALE DI RENDERING
        // --------------------------------------------------------
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            float dt = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();
            ImGuiIO& io = ImGui::GetIO();

            // ----------------------------------------------------
            // A. AGGIORNAMENTO CUBO AUTOMATICO
            // ----------------------------------------------------
            if (state.autoAnimate) {
                animAngle += state.autoSpeed * dt;
            }

            // Calcolo posizione lungo una traiettoria ellittica/circolare con oscillazione verticale
            state.autoPos.x = state.autoRadius * std::cos(animAngle);
            state.autoPos.y = state.autoHeight + 0.4f * std::sin(animAngle * 2.5f);
            state.autoPos.z = state.autoRadius * std::sin(animAngle);

            glm::mat4 autoModel(1.0f);
            autoModel = glm::translate(autoModel, state.autoPos);
            // Rotazione del cubo su se stesso mentre orbita
            autoModel = glm::rotate(autoModel, animAngle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            autoModel = glm::rotate(autoModel, animAngle * 1.2f, glm::vec3(1.0f, 0.0f, 1.0f));
            autoModel = glm::scale(autoModel, glm::vec3(0.9f));

            scene.instance(autoCube).modelMatrix = autoModel;
            scene.instance(autoCube).color = state.autoColor;

            // ----------------------------------------------------
            // B. AGGIORNAMENTO CUBO MANUALE (INPUT DA TASTIERA)
            // Se ImGui non sta catturando la tastiera, i tasti muovono il cubo
            // ----------------------------------------------------
            if (!io.WantCaptureKeyboard) {
                float manualSpeed = 4.5f * dt;
                // I / K: avanti / indietro lungo l'asse Z
                if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) state.manualPos.z -= manualSpeed;
                if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) state.manualPos.z += manualSpeed;
                // J / L: sinistra / destra lungo l'asse X
                if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) state.manualPos.x -= manualSpeed;
                if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) state.manualPos.x += manualSpeed;
                // U / O: alza / abbassa lungo l'asse Y
                if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) state.manualPos.y += manualSpeed;
                if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) state.manualPos.y -= manualSpeed;

                // Tasti N / M: rotazione manuale attorno all'asse Y
                if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) state.manualRot.y -= 90.0f * dt;
                if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) state.manualRot.y += 90.0f * dt;
            }

            // Calcolo Model Matrix del cubo manuale: Traslazione * Rotazione * Scala
            glm::mat4 manualModel(1.0f);
            manualModel = glm::translate(manualModel, state.manualPos);
            manualModel = glm::rotate(manualModel, glm::radians(state.manualRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
            manualModel = glm::rotate(manualModel, glm::radians(state.manualRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
            manualModel = glm::rotate(manualModel, glm::radians(state.manualRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
            manualModel = glm::scale(manualModel, glm::vec3(state.manualScale));

            scene.instance(manualCube).modelMatrix = manualModel;
            scene.instance(manualCube).color = state.manualColor;

            // ----------------------------------------------------
            // C. CONTROLLI DELLA TELECAMERA
            // ----------------------------------------------------
            if (!io.WantCaptureKeyboard) {
                float camSpeed   = 6.0f * dt;
                float orbitSpeed = 1.8f * dt;

                // W/S/A/D/Q/E: spostamento locale della camera
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveLocal(camSpeed, 0.0f, 0.0f);
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveLocal(-camSpeed, 0.0f, 0.0f);
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveLocal(0.0f, -camSpeed, 0.0f);
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveLocal(0.0f, camSpeed, 0.0f);
                if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.moveLocal(0.0f, 0.0f, camSpeed);
                if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveLocal(0.0f, 0.0f, -camSpeed);

                // Frecce: orbita attorno al target
                if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) camera.orbit(-orbitSpeed, 0.0f);
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.orbit(orbitSpeed, 0.0f);
                if (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) camera.orbit(0.0f, orbitSpeed);
                if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) camera.orbit(0.0f, -orbitSpeed);

                // Tasti + e - per lo zoom
                if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS ||
                    glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
                    camera.zoom(6.0f * dt);
                if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ||
                    glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
                    camera.zoom(-6.0f * dt);
            }

            // Orbita e zoom con il mouse (se il mouse non è sul pannello ImGui)
            if (!io.WantCaptureMouse) {
                if (Input::mouseDragging) {
                    double dx = Input::mouseX - Input::lastMouseX;
                    double dy = Input::mouseY - Input::lastMouseY;
                    camera.orbit(static_cast<float>(dx) * 0.005f, static_cast<float>(dy) * 0.005f);
                    Input::lastMouseX = Input::mouseX;
                    Input::lastMouseY = Input::mouseY;
                }
                if (std::abs(Input::scrollY) > 0.001) {
                    camera.zoom(static_cast<float>(Input::scrollY) * 0.7f);
                    Input::scrollY = 0.0;
                }
            }

            // ----------------------------------------------------
            // D. RENDERING DELLA SCENA OPENGL
            // ----------------------------------------------------
            int width = 0, height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            if (width == 0 || height == 0) continue;

            glViewport(0, 0, width, height);
            glClearColor(state.background.r, state.background.g, state.background.b, state.background.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer.draw(scene, camera, float(width) / float(height), state);

            // ----------------------------------------------------
            // E. RENDERING DELL'INTERFACCIA IMGUI
            // ----------------------------------------------------
            gui.beginFrame();
            gui.draw();
            gui.render();

            glfwSwapBuffers(window);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
