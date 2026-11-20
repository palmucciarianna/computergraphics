// ESERCITAZIONE 5 - TEMPLATE STUDENTI
// Proiezione ortografica e matrici Model inizialmente identita.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"
#include "ShapeGenerator.h"
#include "Scene.h"
#include "Renderer.h"
#include "Input.h"
#include "Gui.h"

glm::mat4 makeModel(float dx, float dy, float sx, float sy, float angle)
{
    //matrice identità
    glm::mat4 model(1.0);

    //Traslazione

    model = glm::translate(model, glm::vec3(dx, dy, 0.0));
    //Rotazione
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    //Scalatura
    model = glm::scale(model,glm::vec3(sx, sy, 1.0));

    return model;

}

void updateOrthoProjection(
    GLFWwindow* window,
    Renderer& renderer)
{
    int width = 0;
    int height = 0;

    // Legge le dimensioni effettive del framebuffer in pixel.
    glfwGetFramebufferSize(window, &width, &height);

    // Evita la divisione per zero.
    if (width <= 0 || height <= 0)
        return;

    // Calcola il rapporto larghezza/altezza.
    const float aspect =
        static_cast<float>(width) / height;

    // Definisce la regione visibile del mondo.
    float halfWidth = 100.0f;
    float halfHeight = 100.0f;

    // Adatta la regione visibile alle proporzioni
    // del framebuffer senza deformare le figure.
    if (aspect >= 1.0f)
        halfWidth = 100.0f * aspect;
    else
        halfHeight = 100.0f / aspect;

    // Costruisce la matrice di proiezione ortografica.
    const glm::mat4 projection = glm::ortho(
        -halfWidth, halfWidth,
        -halfHeight, halfHeight,
        -1.0f, 1.0f
    );

    // Memorizza la matrice nella classe Renderer.
    renderer.setProjectionMatrix(projection);
}

void buildGrid(Scene& scene, MeshData& butterfly, MeshData& heart, int numRows, int numCols )
{
    float left = -90.0, right = 90.0, bottom = -90.0, top = 90.0;

    float cellWidth = (right - left) / numCols;
    float cellHeight = (top - bottom) / numRows;

    float sx = 16.0, sy = 16.0;

    scene.clear();

    for (int row = 0; row < numRows; ++row)
    {
        for (int col=0; col<numCols; ++col)
        {
            float x = left + (col + 0.5) * cellWidth;
            float y = top - (row + 0.5) * cellHeight;
            MeshData& shape = (row % 2) ? butterfly : heart; //shape è la farfalla per le righe pari, cuore per le righe dispari
            std::size_t index = scene.add(shape, false);
            float sx = (row % 2) ? 40.0 : 15.0;  // Seleziona un fattore di scala diverso a seconda della shape
            scene[index].setModelMatrix(makeModel(x, y, sx, sx, 0.0)); //Assegna la matrice di modellazione opportuna
        }
    }
}

int main()
{
    // ========================================================================
    // 1. GLFW
    // ========================================================================

    if (!glfwInit())
    {
        std::cout
            << "Errore inizializzazione GLFW"
            << std::endl;

        return -1;
    }


    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

#ifdef __APPLE__
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
    );
#endif


    // ========================================================================
    // 2. FINESTRA
    // ========================================================================

    GLFWwindow* window =
        glfwCreateWindow(800, 800, "Esercitazione 5 - Proiezione ortografica e Model Matrix", nullptr, nullptr);


    if (!window)
    {
        glfwTerminate();

        return -1;
    }


    glfwMakeContextCurrent(window);


    // ========================================================================
    // 3. GLAD
    // ========================================================================

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cout
            << "Errore caricamento GLAD"
            << std::endl;

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }


    // ========================================================================
    // 4. CALLBACK DEL PROGETTO
    // ========================================================================

    glfwSetKeyCallback(window, Input::keyCallback);

    glfwSetCursorPosCallback(window, Input::cursorPositionCallback);

    glfwSetFramebufferSizeCallback(window, Input::framebufferSizeCallback);


    // ========================================================================
    // 5. GUI
    // ========================================================================
    //
    // Inizializziamo ImGui DOPO aver creato il contesto OpenGL.
    // Il backend GLFW concatena le callback ImGui con quelle gia' registrate.
    //

    Gui gui;

    gui.initialize(window);


    // ========================================================================
    // 6. SHADER
    // ========================================================================

    Shader shader;

    if (!shader.load("shader/vertexshaderC.glsl", "shader/fragmentshaderC.glsl")) {
        std::cerr << "Errore nel caricamento degli shader" << std::endl;
        gui.shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    shader.use();


    // ========================================================================
    // 7. COSTRUZIONE DELLA SCENA
    // ========================================================================

    Scene scene;

    Renderer renderer(shader);

     
     // Aggiorna la proiezione se il framebuffer cambia dimensione.
    updateOrthoProjection(window, renderer);


    // ========================================================================
    // GEOMETRIE LOCALI E MATRICI DI MODELLAZIONE
    // ========================================================================
    // Tutte le forme sono costruite in coordinate locali.
    // Inizialmente Model = I per ogni geometria: le coordinate locali
    // coincidono con quelle del mondo.
    // ATTIVITA': introdurre scala, traslazione, rotazione e animazione.

   

    // ------------------------------------------------------------------------
    // CUORE - coordinate locali
    // ------------------------------------------------------------------------
    MeshData heart = ShapeGenerator::createHeart(
        0.0f, 0.0f, 0.05f, 0.05f, 60,
        glm::vec4(1.0f, 1.0f, 1.0f, 0.6f),
        glm::vec4(1.0f, 0.0f, 0.0f, 0.6f)
    );
 
    // ------------------------------------------------------------------------
    // FARFALLA - coordinate locali
    // ------------------------------------------------------------------------
    MeshData butterfly = ShapeGenerator::createButterfly(
        0.0f, 0.0f, 0.1f, 0.1f, 120,
        glm::vec4(1.0f, 1.0f, 1.0f, 0.6f),
        glm::vec4(1.0f, 0.0f, 0.5f, 0.6f)
    );

    //Chiamo il metodo gui.numRows() e gui.numCols() che espone le variabili delle dimensione della griglia aggiornate con slider nell'interfaccia
    int previousRows = gui.numRows();
    int previousCols = gui.numCols();

    //Costruisce la scena
    buildGrid(scene, butterfly, heart, previousRows, previousCols);


    // ========================================================================
    // 8. BLENDING
    // ========================================================================

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // ========================================================================
    // 9. RENDER LOOP
    // ========================================================================

    while (!glfwWindowShouldClose(window))
    {


        // --------------------------------------------------------------------
        // GUI: INIZIO FRAME E COSTRUZIONE DEI CONTROLLI
        // --------------------------------------------------------------------

        
         // Aggiorna la proiezione se il framebuffer cambia dimensione.
        updateOrthoProjection(window, renderer);

        gui.beginFrame();

        gui.draw();

        //Ricosrtuisce la griglia nel caso in cui siano cambiate le sue dimensioni

        if (gui.numRows()!=previousRows || gui.numCols()!=previousCols)
        {
            previousRows = gui.numRows();
            previousCols = gui.numCols();
            buildGrid(scene, butterfly, heart, previousRows, previousCols);

        }

        // --------------------------------------------------------------------
        // PREPARAZIONE DEL FRAME OPENGL
        // --------------------------------------------------------------------
        //
        // Il colore non e' piu' scritto direttamente nel main.
        // Viene scelto dall'utente nella GUI e letto dal Renderer.
        //

        renderer.beginFrame(
            gui.clearColor()
        );


        renderer.setWireframe(
            gui.wireframe()
        );
       
        // --------------------------------------------------------------------
        // DISEGNO DELLA SCENA
        // --------------------------------------------------------------------

        renderer.drawScene(scene);


        renderer.setWireframe(
            false
        );

        // --------------------------------------------------------------------
        // DISEGNO DELLA GUI
        // --------------------------------------------------------------------
        //
        // Deve avvenire DOPO la scena perche' l'interfaccia resti visibile
        // sopra il contenuto OpenGL.
        //

        gui.render();


        // --------------------------------------------------------------------
        // PRESENTAZIONE DEL FRAME
        // --------------------------------------------------------------------

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    // ========================================================================
    // 10. CLEANUP
    // ========================================================================

    scene.clear();

    shader.destroy();


    // ImGui deve essere chiuso mentre il contesto OpenGL esiste ancora.
    gui.shutdown();


    glfwDestroyWindow(window);

    glfwTerminate();


    return 0;
}
