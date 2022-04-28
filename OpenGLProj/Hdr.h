#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class Hdr
{
public:
    Hdr();
    ~Hdr();

    void Init(GLuint w, GLuint h);
    void Bind();
    void Unbind();
    void Bloom();
    void RenderQuad();	// Render NDC screenquad

public:
    std::shared_ptr<Shader> m_pShaderBlur{ nullptr };
    std::shared_ptr<Shader> m_pShaderBloomHDR{ nullptr };

    GLuint m_hdrFBO{ 0 };
    GLuint m_colorBuffers[2]{ 0 };
    GLuint m_pingpongFBO[2]{ 0 }; // ping-pong-framebuffer for blurring 
    GLuint m_pingpongColorbuffers[2]{ 0 };

    //settings in render
    bool m_bloom = true;
    //bool m_bloomKeyPressed = false;
    float m_exposure = 2.0f;

    GLuint m_quadVAO{ 0 };
    GLuint m_quadVBO{ 0 };
};
