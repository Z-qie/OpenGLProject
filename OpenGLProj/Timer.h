#pragma once
#include <chrono>
#include <iostream>

struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> m_Start, m_End;
    std::chrono::duration<float> m_Duration;
    

    ~Timer() {
        m_Duration = m_End - m_Start;
    }

    void Start() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    void End() {
        m_End = std::chrono::high_resolution_clock::now();
        m_Duration = m_End - m_Start;
    }

    void PrintFrameRate() {
        std::cout << "Frame Rate: " << 1.0 / m_Duration.count() << std::endl;
    }

    float DeltaTime() {
        return m_Duration.count();
    }
}; 