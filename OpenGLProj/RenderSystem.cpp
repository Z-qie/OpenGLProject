

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "RenderSystem.h"
#include "Camera.h"
#include "Scene.h"

RenderSystem::RenderSystem(GLuint w, GLuint h)
    :m_width(w), m_height(h) {
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}


void RenderSystem::Init(const Scene& scene) {
    SetDefaultState();

    // Init UBO
    // Create uniform buffer object for projection and view matrices
    // so same data shared to multiple shader programs.
    // view and proj
    glGenBuffers(1, &m_uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, (GLuint)UboIndex::UBOMatrices, m_uboMatrices, 0, 2 * sizeof(glm::mat4));
    // dirlight
    glGenBuffers(1, &m_uboIndexDirLight);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboIndexDirLight);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, (GLuint)UboIndex::UBODirLight, m_uboIndexDirLight, 0, 4 * sizeof(glm::vec4));
    // point light
    glGenBuffers(scene.m_numFish, &m_uboIndexPointLights);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboIndexPointLights);
    glBufferData(GL_UNIFORM_BUFFER, scene.m_numFish * 5 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, (GLuint)UboIndex::UBOPointLights, m_uboIndexPointLights, 0, scene.m_numFish * 5 * sizeof(glm::vec4));

    // Init skybox
    m_skybox.Init();

    // Init HDR
    m_HDR.Init(m_width, m_height);
}


void RenderSystem::Update(const Camera& camera, const Scene& scene) {

    // Update matrix inside UBO
    m_projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)m_width / (float)m_height, camera.Near, camera.Far);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), value_ptr(m_projMatrix));

    glm::mat4 view = camera.GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

    // update lights     
    // dirlight
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboIndexDirLight);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(scene.m_DirLights[0]->direction));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(scene.m_DirLights[0]->ambient));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec4), glm::value_ptr(scene.m_DirLights[0]->diffuse));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3, sizeof(glm::vec4), glm::value_ptr(scene.m_DirLights[0]->specular));

    // point lights
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboIndexPointLights);
    float offset = 0;
    for (int i = 0; i < scene.m_numFish; i++)
    {
        offset = i * 5 * sizeof(glm::vec4);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(scene.m_PointLights[i]->position));
        glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::vec4) * 1, sizeof(glm::vec4), &(scene.m_PointLights[i]->constant));
        glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::vec4) * 2, sizeof(glm::vec4), glm::value_ptr(scene.m_PointLights[i]->ambient));
        glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::vec4) * 3, sizeof(glm::vec4), glm::value_ptr(scene.m_PointLights[i]->diffuse));
        glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::vec4) * 4, sizeof(glm::vec4), glm::value_ptr(scene.m_PointLights[i]->specular));
    }

}


void RenderSystem::Render(const Camera& camera, const Scene& scene) {
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // first pass *******************************************************************
    //  render scene into floating point framebuffer
    m_HDR.Bind();


    for (auto& entity : scene.m_pSceneEntities)
        entity->DrawSelfAndChild(camera); // set up their shaders here
    // Skybox
    m_skybox.Draw(glm::mat4(glm::mat3(camera.GetViewMatrix())), m_projMatrix);    // remove translation from the view matrix
   
    
    m_HDR.Unbind();


    // second pass: Post-Processing *******************************************************************
    // Bloom
    m_HDR.Bloom();

}


void RenderSystem::SetDefaultState() {
    //configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

// I have changed resouces as shared pointer for convienence
void RenderSystem::Shutdown() const {
#ifdef _DEBUG
    std::cout << "Render shut down\n";
#endif
}