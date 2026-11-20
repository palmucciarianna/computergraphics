#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include "ShapeGenerator.h"
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "GuiLayer.h"
#include <imgui.h>


// Solo lo stato temporaneo degli input: nessun puntatore a Scene/Camera/GuiLayer.
// Gli oggetti sono passati direttamente per riferimento a handleInput().
struct InputState {
    bool mouseDown = false;
    bool draggingScene = false;
    bool previousTab = false;
    bool previousSpace = false;
    bool previousDelete = false;
    double lastX = 0.0;
    double lastY = 0.0;
};

// Questa e' una normale funzione C++, non una callback GLFW.
// Puo' quindi ricevere direttamente Camera, Scene e GuiLayer per riferimento.
void handleInput(GLFWwindow* window, Camera& camera, Scene& scene,
                 GuiLayer& gui, InputState& input, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    // ImGui gestisce i click sui propri controlli e sui popup.
    bool mouseOnGui = ImGui::GetIO().WantCaptureMouse ||
                      ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    bool keyboardOnGui = ImGui::GetIO().WantCaptureKeyboard;
    if (!keyboardOnGui) {
        bool tab = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
        if (tab && !input.previousTab) gui.toggleNavigationMode();
        input.previousTab = tab;
        bool space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (space && !input.previousSpace) gui.toggleWireframe();
        input.previousSpace = space;
        // Navigazione della camera: funzionalita' gia' implementata.
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.orbit(-dt, 0);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.orbit(dt, 0);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.orbit(0, dt);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.orbit(0, -dt);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveLocal(5*dt,0,0);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveLocal(-5*dt,0,0);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveLocal(0,-5*dt,0);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveLocal(0,5*dt,0);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.moveLocal(0,0,5*dt);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveLocal(0,0,-5*dt);

        // TODO 6 - Gestire l'istanza selezionata con la tastiera.
        // 1. Tasti 1/2/3: impostare l'asse X/Y/Z tramite gui.setAxis().
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
            gui.setAxis("X");
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
            gui.setAxis("Y");
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
            gui.setAxis("Z");
        }
        // 2. Leggere gui.selected() e verificarne la validita'.
        int selected = gui.selected();
        if (selected >= 0 && selected < static_cast<int>(scene.instances().size())) {
        // 3. Accedere a Instance& tramite scene.instance(...).
            // 3. Leggere gui.operation() e agire di conseguenza:
            Instance& selectedInstance = scene.instance(selected);
            std::string operation = gui.operation();
            glm::vec3 axis(0);
            if (gui.axis() == "X") axis.x = 1;
            else if (gui.axis() == "Y") axis.y = 1; // axis = glm::vec3(0,1,0);
            else if (gui.axis() == "Z") axis.z = 1;
            else axis = glm::vec3(0,1,0); // default Y

            bool changed = false;
            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
                selectedInstance.position += axis * (3.0f * dt);
                gui.setOperation("Trasla");
                changed = true;
            }
            else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                selectedInstance.rotation += axis * glm::degrees(dt);
                gui.setOperation("Ruota");
                changed = true;
            }
            else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
                selectedInstance.scale *= (1.0f + 0.5f * dt);
                selectedInstance.scale = glm::max(selectedInstance.scale, glm::vec3(0.01f));
                gui.setOperation("Scala");
                changed = true;
            }

            if (changed) {
                selectedInstance.updateModelMatrix();
            }

            // CANC / BACKSPACE: eliminazione dell'istanza selezionata
            bool del = (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS || 
                        glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
            if (del && !input.previousDelete) {
                scene.removeInstance(selected);
                gui.setSelected(-1);
            }
            input.previousDelete = del;

            /*
            // Approccio alternativo: gestione disaccoppiata basata sul valore di gui.operation()
            if (operation == "T") {
                selectedInstance.position += axis * (3.0f * dt);
                selectedInstance.updateModelMatrix();
                gui.setOperation("Trasla");
            }
            else if (operation == "R") {
                selectedInstance.rotation += axis * glm::degrees(dt);
                selectedInstance.updateModelMatrix();
                gui.setOperation("Ruota");
            }
            else if (operation == "G") {
                selectedInstance.scale *= 1.0f + 0.5f * dt;
                selectedInstance.scale = glm::max(selectedInstance.scale, glm::vec3(0.01f));
                selectedInstance.updateModelMatrix();
                gui.setOperation("Scala");
            }
            */
        }
    }

    // TODO 7 - Orbita della camera trascinando il mouse sulla scena.
    // La lista ImGui e' l'unico posto in cui si selezionano istanze.
    // 1. Leggere la posizione corrente con glfwGetCursorPos(window,...).
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    // 2. Leggere il pulsante sinistro con glfwGetMouseButton(...).
    bool left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    // 3. Riconoscere l'INIZIO della pressione confrontando lo stato
    //    corrente con input.mouseDown.
    if (left && !input.mouseDown) {
        input.draggingScene = !mouseOnGui; // inizio trascinamento solo se non si e' sopra ImGui
    }
    // 4. Iniziare il trascinamento della scena solo fuori da ImGui
    //    (variabile mouseOnGui, gia' calcolata sopra).
    if (left && input.mouseDown && input.draggingScene && !mouseOnGui) {
    // 5. Con trascinamento attivo e gui.navigationMode()==true,
    //    chiamare camera.orbit() usando gli incrementi del mouse:
    //       dx = x - input.lastX;  dy = input.lastY - y.
    //    Fattore di sensibilita' suggerito: 0.007f.
        if(gui.navigationMode()){
            camera.orbit(float(x - input.lastX) * 0.007f, float(input.lastY - y) * 0.007f);
        }
    }
    // 6. Al rilascio azzerare input.draggingScene.
    if (!left) {
        input.draggingScene = false;
    }
    // 7. A ogni frame aggiornare input.mouseDown, lastX e lastY.
    input.mouseDown = left;
    input.lastX = x;
    input.lastY = y;
}

int main() {
    // 1. Inizializzazione della finestra e del contesto OpenGL.
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1100,750,"Scena 3D - versione didattica",nullptr,nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    // Lo scope distrugge le risorse OpenGL prima del contesto GLFW.
    {
        Shader shader("shaders/objects.vert", "shaders/objects.frag");
        Renderer renderer(shader);
        Scene scene;
        Camera camera;

        // 2. GEOMETRIE: le forme sono definite nel riferimento locale.
        std::size_t cubeGeometry = scene.addGeometry(ShapeGenerator::cube());
        std::size_t planeGeometry = scene.addGeometry(ShapeGenerator::plane());
        std::size_t pyramidGeometry = scene.addGeometry(ShapeGenerator::pyramid());
        std::size_t sphereGeometry = scene.addGeometry(ShapeGenerator::sphere());
        std::size_t torusGeometry = scene.addGeometry(ShapeGenerator::torus());
        std::array<std::size_t,5> geometryIds = {
            cubeGeometry,planeGeometry,pyramidGeometry,sphereGeometry,torusGeometry
        };

        // 3. ISTANZE: gli oggetti che compariranno nella scena.
        std::size_t cube = scene.addInstance(cubeGeometry,"Cubo");
        std::size_t plane = scene.addInstance(planeGeometry,"Piano");
        std::size_t pyramid = scene.addInstance(pyramidGeometry,"Piramide");
        std::size_t sphere = scene.addInstance(sphereGeometry,"Sfera");
        std::size_t torus = scene.addInstance(torusGeometry,"Toro");

        // 4. Trasformazioni iniziali: parametri TRS e matrice coerente.
        Instance& cubeObject = scene.instance(cube);
        cubeObject.position = glm::vec3(3.5f,0.5f,2.5f);
        cubeObject.scale = glm::vec3(2.0f);
        cubeObject.updateModelMatrix();

        Instance& planeObject = scene.instance(plane);
        planeObject.position = glm::vec3(0.0f,-1.5f,0.0f);
        planeObject.scale = glm::vec3(100.0f,1.0f,100.0f);
        planeObject.updateModelMatrix();
        planeObject.selectable = false;

        Instance& pyramidObject = scene.instance(pyramid);
        pyramidObject.position = glm::vec3(-1.0f,-1.5f,5.0f);
        pyramidObject.scale = glm::vec3(1.2f);
        pyramidObject.updateModelMatrix();

        Instance& sphereObject = scene.instance(sphere);
        sphereObject.position = glm::vec3(3.5f,5.5f,2.5f);
        sphereObject.scale = glm::vec3(1.5f);
        sphereObject.updateModelMatrix();

        Instance& torusObject = scene.instance(torus);
        torusObject.position = glm::vec3(-2.0f,0.5f,2.5f);
        torusObject.rotation = glm::vec3(90.0f,0.0f,0.0f);
        torusObject.scale = glm::vec3(1.5f,0.5f,1.5f);
        torusObject.updateModelMatrix();

        GuiLayer gui(window,scene,geometryIds);
        InputState input;
        double lastTime = glfwGetTime();

        // 5. CICLO DI RENDERING.
        while (!glfwWindowShouldClose(window)) {
            double time = glfwGetTime();
            float dt = std::min(float(time-lastTime),0.05f);
            lastTime = time;
            glfwPollEvents();
            gui.beginFrame();
            gui.draw();
            handleInput(window, camera, scene, gui, input, dt);

            int width,height;
            glfwGetFramebufferSize(window,&width,&height);
            if (width == 0 || height == 0) continue;
            glViewport(0,0,width,height);
            glClearColor(gui.background().r,gui.background().g,gui.background().b,gui.background().a);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            renderer.draw(scene,camera,float(width)/height,
                          gui.wireframe(),gui.showAnchor(),gui.selected());
            gui.render();
            glfwSwapBuffers(window);
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
