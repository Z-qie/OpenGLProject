#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

class Transform
{
protected:
    glm::mat4 getLocalModelMatrix()
    {
        //rotate around x, y, z axis
        const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        const glm::mat4 roationMatrix = transformY * transformX * transformZ;

        // translation * rotation * scale (TRS matrix)
        return glm::translate(glm::mat4(1.0f), m_pos) * roationMatrix * glm::scale(glm::mat4(1.0f), m_scale);
    }
public:

    void ComputeModelMatrix()
    {
        m_modelMatrix = getLocalModelMatrix();
    }

    void ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
    {
        m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
    }

    void setLocalPosition(const glm::vec3& newPosition)
    {
        m_pos = newPosition;
        m_isDirty = true;
    }

    void setLocalRotation(const glm::vec3& newRotation)
    {
        m_eulerRot = newRotation;
        m_isDirty = true;
    }

    void setLocalScale(const glm::vec3& newScale)
    {
        m_scale = newScale;
        m_isDirty = true;
    }

    const glm::vec3& GetGlobalPosition() const
    {
        return m_modelMatrix[3];
    }

    const glm::vec3& GetLocalPosition() const
    {
        return m_pos;
    }

    const glm::vec3& getLocalRotation() const
    {
        return m_eulerRot;
    }

    const glm::vec3& getLocalScale() const
    {
        return m_scale;
    }

    const glm::mat4& GetModelMatrix() const
    {
        return m_modelMatrix;
    }

    glm::vec3 GetRight() const
    {
        return m_modelMatrix[0];
    }

    glm::vec3 GetUp() const
    {
        return m_modelMatrix[1];
    }

    glm::vec3 getBackward() const
    {
        return m_modelMatrix[2];
    }

    glm::vec3 GetForward() const
    {
        return -m_modelMatrix[2];
    }

    glm::vec3 getGlobalScale() const
    {	             // x, y, z
        return { glm::length(GetRight()), glm::length(GetUp()), glm::length(getBackward()) };
    }

    bool IsDirty() const
    {
        return m_isDirty;
    }
private:
    //Local space information
    glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
    glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

    //Global space informaiton concatenate in matrix
    // [x y z global]
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    //Dirty flag: dirty means the local space is modified
    bool m_isDirty = true;

};
