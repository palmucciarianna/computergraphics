// TESORO 2D: moto circolare ottenuto aggiornando la Model Matrix.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include "Scene.h"
#include "ShapeGenerator.h"
#include "Shader.h"
#include "Renderer.h"
#include "BackgroundRenderer.h"

// Traslazione di un oggetto nel mondo 2D.
static glm::mat4 translation(float x, float y) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
}

// Scala di un oggetto rispetto al suo riferimento locale.
static glm::mat4 scaling(float x, float y) {
    return glm::scale(glm::mat4(1.0f), glm::vec3(x, y, 1.0f));
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Impossibile inizializzare GLFW\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1000, 700, "Tesoro 2D - Moto circolare", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Impossibile inizializzare GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    try {
        // Gli oggetti OpenGL vengono distrutti PRIMA del contesto GLFW.
        {
            Shader objectShader("shaders/objects.vert", "shaders/objects.frag");
            Shader backgroundShader("shaders/background.vert", "shaders/background.frag");
            Renderer renderer(objectShader);
            BackgroundRenderer background(backgroundShader);
            Scene scene;

            // ============================================================
            // FASE 1 - GEOMETRIE (dati dei vertici, senza posizione nel mondo)
            // ============================================================
            // Una geometria puo' essere condivisa da molte istanze.
            std::size_t chestBodyGeometry = scene.addGeometry(
                ShapeGenerator::rectangle(1.0f, 1.0f, {0.57f, 0.29f, 0.10f}));
            std::size_t chestLidGeometry = scene.addGeometry(
                ShapeGenerator::rectangle(1.0f, 1.0f, {0.77f, 0.42f, 0.15f}));
            std::size_t goldGeometry = scene.addGeometry(
                ShapeGenerator::rectangle(1.0f, 1.0f, {0.98f, 0.77f, 0.28f}));
            std::size_t headGeometry = scene.addGeometry(
                ShapeGenerator::circle(1.0f, 48, {0.98f, 0.77f, 0.57f}));
            std::size_t bodyGeometry = scene.addGeometry(
                ShapeGenerator::triangle({-0.9f, -1.0f}, {0.9f, -1.0f},
                                         {0.0f, 1.0f}, {0.20f, 0.80f, 0.73f}));
            std::size_t orbitGeometry = scene.addGeometry(
                ShapeGenerator::ring(1.0f, 160, {0.38f, 0.65f, 0.75f}));

            // ============================================================
            // FASE 2 - ISTANZE (oggetti da disegnare nella Scene)
            // ============================================================
            // Il tesoro e' composto da sei istanze; quattro di esse
            // condividono la stessa geometria dorata.
            std::size_t chestBody = scene.addInstance(chestBodyGeometry);
            std::size_t chestLid = scene.addInstance(chestLidGeometry);
            std::size_t lock = scene.addInstance(goldGeometry);
            std::size_t lidLock = scene.addInstance(goldGeometry);
            std::size_t leftBand = scene.addInstance(goldGeometry);
            std::size_t rightBand = scene.addInstance(goldGeometry);

            // Un'istanza per l'orbita e due per il personaggio.
            std::size_t orbitInstance = scene.addInstance(orbitGeometry);
            std::size_t bodyInstance = scene.addInstance(bodyGeometry);
            std::size_t headInstance = scene.addInstance(headGeometry);

            // ============================================================
            // FASE 3 - MODEL MATRIX DELLE ISTANZE FERME
            // ============================================================
            // Ogni istanza possiede una propria matrice di modellazione.
            // M = T * S: prima scala i vertici locali, poi li trasla.
            scene.instance(chestBody).modelMatrix =
                translation(0.0f, -3.0f) * scaling(28.0f, 15.0f);
            scene.instance(chestLid).modelMatrix =
                translation(0.0f, 7.0f) * scaling(30.0f, 7.0f);
            scene.instance(lock).modelMatrix =
                translation(0.0f, -3.0f) * scaling(4.0f, 15.0f);
            scene.instance(lidLock).modelMatrix =
                translation(0.0f, 7.0f) * scaling(4.0f, 7.0f);
            scene.instance(leftBand).modelMatrix =
                translation(-12.0f, -3.0f) * scaling(2.0f, 15.0f);
            scene.instance(rightBand).modelMatrix =
                translation(12.0f, -3.0f) * scaling(2.0f, 15.0f);

            // Il tesoro e' centrato in (0,0); la circonferenza
            // e' una geometria unitaria scalata al raggio scelto.
            const glm::vec2 treasureCenter(0.0f, 0.0f);
            const float orbitRadius = 48.0f;
            scene.instance(orbitInstance).modelMatrix =
                translation(treasureCenter.x, treasureCenter.y) *
                scaling(orbitRadius, orbitRadius);

            // ============================================================
            // FASE 4 - TRASFORMAZIONI LOCALI DEL PERSONAGGIO
            // ============================================================
            // Corpo e testa sono parti dello stesso personaggio.
            // Queste matrici rimangono COSTANTI nel tempo.
            glm::mat4 bodyLocal = translation(0.0f, -2.0f) * scaling(6.0f, 7.0f);
            glm::mat4 headLocal = translation(0.0f, 7.0f) * scaling(4.5f, 4.5f);

            const float angularSpeed = 0.55f; // radianti al secondo
            const glm::mat4 projection = glm::ortho(-80.0f,80.0f,-56.0f,56.0f,-1.0f,1.0f);

            // 5. Ciclo di animazione.
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, GLFW_TRUE);

                float time = static_cast<float>(glfwGetTime());
                float angle = angularSpeed * time;

                // Equazioni parametriche della circonferenza.
                float x = treasureCenter.x + orbitRadius * std::cos(angle);
                float y = treasureCenter.y + orbitRadius * std::sin(angle);

                // MODEL MATRIX GLOBALE del personaggio, aggiornata a ogni frame.
                glm::mat4 characterModel = translation(x, y);

                // MODEL MATRIX di ciascuna istanza del personaggio:
                // M_parte = M_personaggio * M_locale_parte.
                scene.instance(bodyInstance).modelMatrix =
                    characterModel * bodyLocal;
                scene.instance(headInstance).modelMatrix =
                    characterModel * headLocal;

                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                if (width <= 0 || height <= 0) continue;
                glViewport(0, 0, width, height);
                glClear(GL_COLOR_BUFFER_BIT);

                // Due programmi shader differenti, in quest'ordine:
                background.draw(time);
                renderer.drawScene(scene, projection);
                glfwSwapBuffers(window);
            }
        } // Qui si liberano VAO, VBO e shader, con il contesto ancora valido.
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << '\n';
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
