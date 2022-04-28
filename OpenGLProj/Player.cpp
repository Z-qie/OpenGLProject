#include "Player.h"
#include "InputSystem.h"
#include "utils.h"

using namespace math;

void Player::Update(const float deltatime) {
    Move(deltatime);
    Animate(deltatime);

    Entity::Update(deltatime);
}


void Player::Move(float deltatime)
{
    if (!m_canMove) return;

    float velocity = m_movementSpeed * deltatime;

    if (InputSystem::IsKeyPressed(GLFW_KEY_W))
    {
        m_transform.setLocalRotation(Lerp(m_transform.getLocalRotation(), glm::vec3(0, 90, 0), m_rotationSpeed * deltatime));
    };
    if (InputSystem::IsKeyPressed(GLFW_KEY_S))
    {
        m_transform.setLocalRotation(Lerp(m_transform.getLocalRotation(), glm::vec3(0, -90, 0), m_rotationSpeed * deltatime));
    }
    if (InputSystem::IsKeyPressed(GLFW_KEY_A))
    {
        m_transform.setLocalRotation(Lerp(m_transform.getLocalRotation(), glm::vec3(0, 180, 0), m_rotationSpeed * deltatime));
    }
    if (InputSystem::IsKeyPressed(GLFW_KEY_D))
    {
        m_transform.setLocalRotation(Lerp(m_transform.getLocalRotation(), glm::vec3(0, 0, 0), m_rotationSpeed * deltatime));
    }

    if (InputSystem::IsKeyPressed(GLFW_KEY_D) || InputSystem::IsKeyPressed(GLFW_KEY_A)
        || InputSystem::IsKeyPressed(GLFW_KEY_W) || InputSystem::IsKeyPressed(GLFW_KEY_S)) {
        m_transform.setLocalPosition(m_transform.GetLocalPosition() - m_transform.GetForward() * velocity);
    }
}

void Player::Animate(float deltatime) {

    //m_PosYOffset = PingPong(glfwGetTime(), 100) / 100.f * 2.f;
    float curPosY = m_transform.GetLocalPosition().y;
    //std::cout << curPosY << ", " << m_IsfloatingUp << std::endl;
    float PosY;

    if (m_IsfloatingUp) {
        if (curPosY >= m_basePosY + m_PosYOffset - 0.01f)
        {
            m_IsfloatingUp = false;
        }
        PosY = Lerp(curPosY, m_basePosY + m_PosYOffset, m_floatSpeed * deltatime);
    }
    else {
        if (curPosY <= m_basePosY + 0.01f)
        {
            m_IsfloatingUp = true;
        }
        PosY = Lerp(curPosY, m_basePosY, m_floatSpeed * deltatime);
    }
    m_transform.setLocalPosition(glm::vec3(m_transform.GetLocalPosition().x, PosY, m_transform.GetLocalPosition().z));
}


