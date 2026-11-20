#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ShapeGenerator.h"
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "AppState.h"

int main() {
    // ------------------------------------------------------------
    // 1. INIZIALIZZAZIONE DI GLFW E DEL CONTESTO OPENGL
    // ------------------------------------------------------------
    if (!glfwInit()) {
        std::cout << "Errore: inizializzazione GLFW non riuscita.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        1100, 750, "Computer Graphics - Prima scena 3D", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Errore: inizializzazione GLAD non riuscita.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    // Le risorse OpenGL vengono distrutte prima del contesto GLFW.
    {
        // --------------------------------------------------------
        // 2. SHADER, RENDERER, SCENA E CAMERA
        // --------------------------------------------------------
        Shader shader("shaders/objects.vert", "shaders/objects.frag");
        Renderer renderer(shader);
        Scene scene;
        Camera camera;
        AppState state;

        // --------------------------------------------------------
        // 3. CREAZIONE DELLE GEOMETRIE
        // In questa prima versione utilizziamo soltanto cubo e piano.
        // --------------------------------------------------------
        std::size_t cubeGeometry = scene.addGeometry(ShapeGenerator::cube());
        std::size_t planeGeometry = scene.addGeometry(ShapeGenerator::plane());

        // TODO 1: aggiungere una terza geometria, per esempio una piramide.
        // Suggerimento: usare ShapeGenerator::pyramid() e scene.addGeometry().

        // --------------------------------------------------------
        // 4. CREAZIONE DELLE ISTANZE
        // Una geometria descrive la forma; un'istanza e' un oggetto
        // concreto collocato nella scena.
        // --------------------------------------------------------
        std::size_t cube = scene.addInstance(cubeGeometry, "Cubo");
        std::size_t plane = scene.addInstance(planeGeometry, "Piano");

        // TODO 2: creare un'istanza della nuova geometria.

        // --------------------------------------------------------
        // 5. MODEL MATRIX: POSIZIONE E DIMENSIONE DEGLI OGGETTI
        // --------------------------------------------------------
        glm::mat4 cubeModel(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.5f, 0.0f));
        scene.instance(cube).modelMatrix = cubeModel;

        glm::mat4 planeModel(1.0f);
        planeModel = glm::translate(planeModel, glm::vec3(0.0f, -1.0f, 0.0f));
        planeModel = glm::scale(planeModel, glm::vec3(8.0f, 1.0f, 8.0f));
        scene.instance(plane).modelMatrix = planeModel;
        scene.instance(plane).selectable = false;

        // TODO 2b: costruire la Model Matrix della nuova istanza.
        // Provare a usare glm::translate e glm::scale.

        // --------------------------------------------------------
        // 6. CICLO PRINCIPALE
        // La camera e' inizialmente fissa. View e Projection sono
        // calcolate dalla classe Camera.
        // --------------------------------------------------------
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // TODO 6: leggere i tasti e chiamare i metodi della Camera.
            // Usare glfwGetKey(window, GLFW_KEY_...) == GLFW_PRESS.
            // Frecce: camera.orbit(...)
            // W/S, A/D, Q/E: camera.moveLocal(...)

            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);

            if (width == 0 || height == 0)
                continue;

            glViewport(0, 0, width, height);
            glClearColor(0.12f, 0.16f, 0.22f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer.draw(scene, camera, float(width) / float(height), state);

            glfwSwapBuffers(window);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
