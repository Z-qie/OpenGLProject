#pragma once

#include <GLFW/glfw3.h>

// this is currently not useful or necessary, but I wrapped it for future extension
class InputSystem
{
public:
    //singleton
    InputSystem(const InputSystem&) = delete;
    void operator=(const InputSystem&) = delete;

    static InputSystem& Instance()
    {
        static InputSystem s_instance;
        return s_instance;
    }

    static void Update() { Instance().IUpdate(); }

    static void SetUpWindow(GLFWwindow* window) {
        Instance().m_window = window;
    }

    //keyboard
    static auto IsKeyPressed(int key)
    {
        return Instance().IIsKeyPressed(key);
    }

    //cursor
    static void UpdateCurosr(float xPos, float yPos) { Instance().IUpdateCurosr(xPos, yPos); }
    static void  UpdateCurosrScroll(float yOffset) { Instance().m_yScrollOffset = yOffset; }
    //cursor status
    static auto IsCursorMoved() { return Instance().IIsCursorMoved(); }
    static auto GetCursorX() { return Instance().IGetCursorX(); }
    static auto GetCursorY() { return Instance().IGetCursorY(); }
    static auto GetCursorOffsetX() { return Instance().IGetCursorOffsetX(); }
    static auto GetCursorOffsetY() { return Instance().IGetCursorOffsetY(); }
    static auto GetCursorScrollY() { return Instance().IGetCursorScrollY(); }

private:
    InputSystem() {}

    void IUpdate() {
        m_cursorMoved = false;
    }

    bool IIsKeyPressed(int key)
    {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    void IUpdateCurosr(float xPos, float yPos) {
        m_cursorMoved = true;
        m_xPrePos = m_xPos;
        m_yPrePos = m_yPos;
        m_xPos = xPos;
        m_yPos = yPos;
    }

    bool IIsCursorMoved() { return m_cursorMoved; }
    float IGetCursorX() { return m_xPos; }
    float IGetCursorY() { return m_yPos; }
    float IGetCursorOffsetX() { return m_xPos - m_xPrePos; }
    float IGetCursorOffsetY() { return m_yPrePos - m_yPos; }
    float IGetCursorScrollY() { return m_yScrollOffset; }
    //window
    GLFWwindow* m_window{ nullptr };

    // Cursor
    bool m_cursorMoved = false;
    float m_xPos{ 0 }, m_yPos{ 0 };
    float m_xPrePos{ 0 }, m_yPrePos{ 0 };
    float m_yScrollOffset{ 0 };
};