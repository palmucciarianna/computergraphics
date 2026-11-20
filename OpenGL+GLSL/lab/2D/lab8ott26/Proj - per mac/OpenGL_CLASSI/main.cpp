// ============================================================================
// main.cpp - GUIDA DIDATTICA
// Punto di ingresso: inizializza GLFW e GLAD, crea shader e scena, avvia il ciclo di rendering e rilascia le risorse.
// ============================================================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"
#include "ShapeGenerator.h"
#include "Scene.h"
#include "Renderer.h"
#include "Gui.h"
#include "Input.h"

// Avvio del programma. Le risorse OpenGL vanno create solo dopo che esiste un contesto corrente.
int main()
{
    // ========================================================================
    // 1. GLFW
    // ========================================================================

    if (!/* Inizializza GLFW prima di creare la finestra. */
    glfwInit())
    {
        std::cout << "Errore inizializzazione GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    // ========================================================================
    // 2. FINESTRA
    // ========================================================================

    GLFWwindow* window = glfwCreateWindow(
        800,
        800,
        "OpenGL - Template classi",
        nullptr,
        nullptr
    );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    // ========================================================================
    // 3. GLAD
    // ========================================================================

    if (!/* Carica le funzioni OpenGL dopo avere reso corrente il contesto. */
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Errore caricamento GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 800);


    // ========================================================================
    // 4. CALLBACK
    // ========================================================================

    glfwSetKeyCallback(window, Input::keyCallback);
    glfwSetCursorPosCallback(window, Input::cursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, Input::framebufferSizeCallback);


    // ========================================================================
    // 5. SHADER
    // ========================================================================

    Shader shader;

    if (!shader.load(
        "shader/vertexshaderC.glsl",
        "shader/fragmentshaderC.glsl"))
    {
        std::cout << "Errore caricamento shader" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    shader.use();


    // ========================================================================
    // 6. SCENA, RENDERER E GUI
    // ========================================================================

    Scene scene;
    Renderer renderer;
    Gui gui;

    gui.initialize(window);


    // ========================================================================
    // 7. TRIANGOLO
    // ========================================================================

    // Punto 2: creazione UNA SOLA VOLTA, prima del render loop.
    // Conserviamo il triangolo originale, ridotto al centro per non sovrapporlo.
    MeshData triangle = ShapeGenerator::createTriangle(
        glm::vec4(1.0f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.2f, 1.0f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.4f, 1.0f, 1.0f));
    for (auto& position : triangle.positions)
        position *= 0.35f;
    scene.add(triangle, false);

    const int segments = 160;
    scene.add(ShapeGenerator::createCircle(
        glm::vec2(-0.5f, 0.45f), 0.30f, segments,
        glm::vec4(1.0f, 0.9f, 0.2f, 1.0f)), false);
    scene.add(ShapeGenerator::createHeart(
        glm::vec2(0.5f, 0.45f), 0.30f, segments,
        glm::vec4(1.0f, 0.3f, 0.5f, 1.0f)), false);
    scene.add(ShapeGenerator::createButterfly(
        glm::vec2(-0.5f, -0.45f), 0.35f, segments,
        glm::vec4(0.2f, 1.0f, 0.7f, 1.0f)), false);
    scene.add(ShapeGenerator::createMoon(
        glm::vec2(0.5f, -0.45f), 0.30f, segments,
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), false);

    // ========================================================================
    // 8. RENDER LOOP
    // ========================================================================

    // Ciclo dei frame: aggiorna i controlli, pulisce lo schermo, disegna la scena e presenta il risultato.
    while (!glfwWindowShouldClose(window))
    {
        /* Inizia il frame dell interfaccia prima di costruire i widget. */
    gui.beginFrame();
        gui.draw();

        renderer.beginFrame(gui.clearColor());
        renderer.setWireframe(gui.wireframe());

        shader.use();
        /* Il Renderer visita tutte le Geometry e ne richiama draw(). */
    renderer.drawScene(scene);

        // ImGui va disegnata in modalita' fill.
        renderer.setWireframe(false);
        /* Disegna la GUI sopra la scena OpenGL. */
    gui.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // ========================================================================
    // 9. CLEANUP
    // ========================================================================

    gui.shutdown();
    scene.clear();
    shader.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
