#include "WindowSystem.h"
#include "InputSystem.h"

void WindowSystem::Update() {
    glfwPollEvents();

    // Check if the window needs to be closed
    if (InputSystem::IsKeyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(m_window)) {
        m_shouldWindowClose = true;
        glfwSetWindowShouldClose(m_window, true);
    }
}

void WindowSystem::SwapBuffers() const {
    glfwSwapBuffers(m_window);
}


void WindowSystem::Shutdown() const {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void WindowSystem::CreateGLFWWindow(int w, int h, const char* title)
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    m_window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
}