#pragma once

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

namespace Input
{
    extern double mouseX;
    extern double mouseY;
    extern double scrollY;
    extern bool mouseDragging;
    extern double lastMouseX;
    extern double lastMouseY;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
}
