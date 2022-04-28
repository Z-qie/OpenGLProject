#include "Entity.h"
#include "RenderSystem.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

void Entity::Update(const float deltatime) {
    //do something...


    UpdateSelfAndChild();
    for (auto&& child : children)
    {
        child->Update(deltatime);
    }
}


template<typename... TArgs>
void Entity::AddChild(TArgs&... args)
{
    children.emplace_back(std::make_unique<Entity>(args...));
    children.back()->parent = this;
}


void Entity::UpdateSelfAndChild()
{
    if (!m_transform.IsDirty())
        return;

    ForceUpdateSelfAndChild();
}

void Entity::ForceUpdateSelfAndChild()
{
    if (m_pParent)
        m_transform.ComputeModelMatrix(m_pParent->m_transform.GetModelMatrix());
    else
        m_transform.ComputeModelMatrix();

    for (auto&& child : children)
    {
        child->ForceUpdateSelfAndChild();
    }
}


void Entity::DrawSelfAndChild(const Camera& camera)
{
    if (m_pShader) { // set up shader
        m_pShader->Use();

        // UBO VP
        GLuint matricesIndex = glGetUniformBlockIndex(m_pShader->ID, "Matrices");
        glUniformBlockBinding(m_pShader->ID, matricesIndex, (GLuint)UboIndex::UBOMatrices); 
        // UBO light
        GLuint DirLightIndex = glGetUniformBlockIndex(m_pShader->ID, "DirLightUBO");
        glUniformBlockBinding(m_pShader->ID, DirLightIndex, (GLuint)UboIndex::UBODirLight);
        // UBO Point Light
        GLuint PointLightIndex = glGetUniformBlockIndex(m_pShader->ID, "PointLightsUBO");
        glUniformBlockBinding(m_pShader->ID, PointLightIndex, (GLuint)UboIndex::UBOPointLights);
        m_pShader->setMat4("model", m_transform.GetModelMatrix());
        m_pShader->setVec3("viewPos", camera.Position);
        m_pShader->setInt("blinn", true);
    }

    if (m_pModel) {
        m_pModel->Draw(m_pShader);
    }

    for (auto&& child : children)
    {
        child->DrawSelfAndChild(camera);
    }
}