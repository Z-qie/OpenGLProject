#pragma once

#include "Entity.h"

class Player : public Entity {

public:
    // for convinience, we set up everything here
    Player() {
        // load source
        m_pShader = std::make_shared<Shader>("resources/shaders/player.vs", "resources/shaders/player.fs");
        m_pModel = std::make_shared<Model>("resources/objects/player/player.obj");

        m_transform.setLocalScale(glm::vec3(0.8f, 0.8f, 0.8f));
        m_transform.setLocalRotation(glm::vec3(0.0f, 90.0f, 0.0f));
        m_transform.setLocalPosition(glm::vec3(40.f, 8.f, 40.f));
    }

    virtual void Update(const float deltatime);
    void Move(float deltatime);
    void Animate(float deltatime);

public:
    bool m_canMove{ false };

    // animation setting
    bool m_IsfloatingUp{ false };
    float m_basePosY{ 8.f};
    float m_PosYOffset{ 1.f };
    float m_floatSpeed{ 1.2f };

    float m_rotationSpeed{10.0f};
    float m_movementSpeed{15.0f};
};