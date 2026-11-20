#include <glad/glad.h>
#include "Input.h"
#include <imgui.h>

namespace Input
{
    double mouseX = 0.0;
    double mouseY = 0.0;
    double scrollY = 0.0;
    bool mouseDragging = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        (void)scancode;
        (void)mods;

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        mouseX = xpos;
        mouseY = static_cast<double>(height) - ypos;
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        (void)window;
        (void)mods;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                mouseDragging = true;
                lastMouseX = mouseX;
                lastMouseY = mouseY;
            } else if (action == GLFW_RELEASE) {
                mouseDragging = false;
            }
        }
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        (void)window;
        (void)xoffset;
        scrollY += yoffset;
    }

    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        (void)window;
        glViewport(0, 0, width, height);
    }
}
