// ============================================================================
// Triangolo OpenGL con interfaccia ImGui minima
//
// L'interfaccia grafica permette di modificare SOLO il colore dello sfondo.
// ============================================================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Gui.h"
#include "ShaderMaker.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void INIT_SHADER(void);
void INIT_VAO(void);

static unsigned int programId;
unsigned int VAO;
unsigned int VBO;

// Colore iniziale dello sfondo. La GUI modifica direttamente questa variabile.
ImVec4 clear_color = ImVec4(0.45f, 0.0f, 0.0f, 1.0f);

// Vertici del triangolo.
// Ogni vertice contiene: posizione (x,y,z) + colore (r,g,b,a).
float vertices[] = {
    // posizione             // colore
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
     0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f
};

int main(void)
{
    GLFWwindow* window;

    // 1. Inizializzazione di GLFW
    if (!glfwInit())
        return -1;

    // 2. Richiesta di un contesto OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif

    // 3. Creazione della finestra
    window = glfwCreateWindow(800, 800, "OpenGL - colore sfondo", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create the window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 4. Inizializzazione di GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL function pointers!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 5. Inizializzazione delle risorse OpenGL e di ImGui
    INIT_SHADER();
    INIT_VAO();
    Initialize_IMGUI(window);

    // 6. Render loop
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        // Il colore usato per cancellare il framebuffer viene scelto dalla GUI.
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // Costruisce l'interfaccia ImGui del frame corrente.
        // In questa versione contiene unicamente il selettore del colore di sfondo.
        my_interface();

        // Disegno del triangolo
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Disegno dell'interfaccia in primo piano
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 7. Cleanup
    close_GUI();
    glDeleteProgram(programId);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();

    return 0;
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void INIT_SHADER(void)
{
    char* vertexShader = (char*)"vertex_shader_PT.glsl";
    char* fragmentShader = (char*)"fragment_shader_PT.glsl";

    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
    glUseProgram(programId);
}

void INIT_VAO(void)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attributo 0: posizione (x, y, z)
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        7 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Attributo 1: colore (r, g, b, a)
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        7 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);
}
