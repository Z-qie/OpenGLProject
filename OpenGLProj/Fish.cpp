#include "Fish.h"
#include "InputSystem.h"
#include "utils.h"

void Fish::Update(const float deltatime) {
    //do something...
    //control

    m_pPointLight->position = m_transform.GetGlobalPosition();


    //Move(deltatime);
    if (InputSystem::IsKeyPressed(GLFW_KEY_P)) {
        m_canMove = !m_canMove;
    }
    if (m_canMove)
        Move(deltatime);
    //Animate(deltatime);
    Entity::Update(deltatime);
}


void Fish::Move(float deltatime)
{

    float velocity = m_movementSpeed * deltatime;
    m_curTime = glfwGetTime();
    if (m_nextTurnTime - m_curTime <= 0) {
        m_nextTurnTime = m_curTime + m_turnInterval;
        Turn();
    }


    m_transform.setLocalRotation(math::Lerp(m_transform.getLocalRotation(), m_rotationTarget, m_rotationSpeed * deltatime));
    m_transform.setLocalPosition(m_transform.GetLocalPosition() + m_transform.GetRight() * velocity);

    float curPoxX = m_transform.GetLocalPosition().x;
    float curPoxY = m_transform.GetLocalPosition().y;
    float curPoxZ = m_transform.GetLocalPosition().z;

    curPoxY = math::Lerp(curPoxY, m_heightTarget, m_yawSpeed * deltatime);
    m_transform.setLocalPosition(glm::vec3(curPoxX, curPoxY, curPoxZ));

    if (curPoxX < -5)
        m_transform.setLocalPosition(glm::vec3(
            m_movingZoneX, curPoxY, curPoxZ));


    if (curPoxX > m_movingZoneX)
        m_transform.setLocalPosition(glm::vec3(
            -5, curPoxY, curPoxZ));

    if (curPoxZ < -5)
        m_transform.setLocalPosition(glm::vec3(
            curPoxX, curPoxY, m_movingZoneY));

    if (curPoxZ > m_movingZoneY)
        m_transform.setLocalPosition(glm::vec3(
            curPoxX, curPoxY, -5));
}


void Fish::Turn() {
    m_rotationTarget = glm::vec3(0, rand() % 360 - 180, 0);
    m_heightTarget = rand() % (int)(m_movingZoneZUp - m_movingZoneZDown) + m_movingZoneZDown;
}


