#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

class WindowSystem {
    friend class Engine;

public:
    WindowSystem(GLuint w, GLuint h, const char* title)
        :m_width(w), m_height(h), m_title(title) {
        CreateGLFWWindow(w, h, title);
    }

    void Update(); // check if close window
    void SwapBuffers() const;
    void Shutdown() const; // close window

private:
    void CreateGLFWWindow(int w, int h, const char* title);
   
    GLFWwindow* m_window{ nullptr };
    GLuint m_width{ 2000 };
    GLuint m_height{ 1600 };
    const char* m_title;
    bool m_shouldWindowClose{ false };
};
