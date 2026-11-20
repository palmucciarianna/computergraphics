// Progetto1_Finestra //

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Colore corrente dello sfondo (RGBA)
float backgroundColor[4] = { 0.15f, 0.20f, 0.30f, 1.0f };



int main()
{
    // 1. Inizializzazione di GLFW
    if (!glfwInit())
    {
        std::cerr << "Errore: impossibile inizializzare GLFW." << std::endl;
        return -1;
    }

    // 2. Configurazione del contesto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // 3. Creazione di UNA SOLA finestra in modalita' windowed
    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "OpenGL - Click per cambiare colore",
        nullptr,   // nullptr => non fullscreen
        nullptr
    );

    if (!window)
    {
        std::cerr << "Errore: impossibile creare la finestra GLFW." << std::endl;
        glfwTerminate();
        return -1;
    }

    // 4. Rende corrente il contesto OpenGL associato alla finestra
    glfwMakeContextCurrent(window);

    // 5. Caricamento delle funzioni OpenGL tramite GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Errore: impossibile inizializzare GLAD." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // 6. Registrazione della callback del mouse
   

    // 7. Game/render loop
    while (!glfwWindowShouldClose(window))
    {
        // Colore di sfondo corrente
        glClearColor(
            backgroundColor[0],
            backgroundColor[1],
            backgroundColor[2],
            backgroundColor[3]
        );

        glClear(GL_COLOR_BUFFER_BIT);

        // Visualizza il frame appena renderizzato
        glfwSwapBuffers(window);

        // Gestisce gli eventi di input (mouse, tastiera, chiusura finestra, ...)
        glfwPollEvents();
    }

    // 8. Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
