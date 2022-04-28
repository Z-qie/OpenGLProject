#pragma once

#include "Entity.h"
#include "Light.h"

class Fish : public Entity {

public:
    // for convinience, we set up everything here
    Fish() {
        // load source
        m_pShader = std::make_shared<Shader>("resources/shaders/fish.vs", "resources/shaders/fish.fs");
        m_pModel = std::make_shared<Model>("resources/objects/fish/fish.obj");
        //srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed

        //int posX = rand() % 60 + 10;
        //int posY= rand() % 6 + 4;
        //int posZ = rand() % 60 + 10;

        m_transform.setLocalPosition(glm::vec3(40.f, 4.f, 40.f));

        //scale = 5;
        //m_transform.setLocalScale(glm::vec3(scale, scale, scale));
        m_transform.setLocalRotation(glm::vec3(0.0f, 0.f, 0.0f));


        m_pPointLight = std::make_shared <PointLight>(glm::vec3(2,2,2), // position
            .0f,                           // constant;
            20.0f,                          // linear;
            20.f,                          // quadratic;
            glm::vec3(0.2f, 0.2f, 0.2f), //ambient
            glm::vec3(9.f, 9.f, 9.f),    //diffuse
            glm::vec3(9.f, 9.f, 9.f)     //specular
        );
    }

    virtual void Update(const float deltatime);
    void Move(float deltatime);
    void Turn();

public:
    // restriction
    bool m_canMove{ false };
    float m_movingZoneX{ 80 };
    float m_movingZoneY{ 80 };
    float m_movingZoneZUp{ 12.f };
    float m_movingZoneZDown{ 4 };
    
    // animation setting
    float m_rotationSpeed{ 1.5f };
    float m_movementSpeed{ 10.f };
    float m_yawSpeed{ 1.f };
    
    // timer
    glm::vec3 m_rotationTarget{ 0.f,0.f, 0.f };
    float m_heightTarget{ 0 };
    float m_nextTurnTime{ 0 };
    float m_curTime{ 0 };
    float m_turnInterval{ .3f};

    // light
    std::shared_ptr<PointLight> m_pPointLight;
};