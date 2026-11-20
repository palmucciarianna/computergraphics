#pragma once

#include <GLFW/glfw3.h>

// Inizializza ImGui e collega l'interfaccia a GLFW/OpenGL.
void Initialize_IMGUI(GLFWwindow* window);

// Costruisce l'interfaccia del frame corrente.
// In questa versione contiene SOLO il controllo del colore dello sfondo.
void my_interface(void);

// Libera le risorse utilizzate da ImGui.
void close_GUI(void);
