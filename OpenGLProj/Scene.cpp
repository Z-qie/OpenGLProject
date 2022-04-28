#include <iostream>

#include "Fish.h"
#include "Scene.h"
#include "InputSystem.h"


void Scene::Init(Camera& camera) {
#ifdef _DEBUG
    std::cout << "Loading scene..." << std::endl;
#endif
    // camera ********************************************************************************
    m_pCamera = &camera;

    // add tester
  /*  auto testShader = std::make_shared<Shader>("resources/shaders/tester.vs", "resources/shaders/tester.fs");
    auto testModel = std::make_shared<Model>("resources/objects/backpack/backpack.obj");

    m_pTest = std::make_shared<Entity>(testShader, testModel);
    AddEntity(m_pTest);*/



    // add tile map ********************************************************************************
    m_pTilemap = std::make_shared<Tilemap>();
    AddEntity(m_pTilemap);

    // add player and camera follow ********************************************************************************
    m_pPlayer = std::make_shared<Player>();
    AddEntity(m_pPlayer);
    m_pCamera->Follow(m_pPlayer->m_transform);
    m_pPlayer->m_canMove = true;
    m_pTilemap->m_PlayerTransform = &(m_pPlayer->m_transform);

    m_pCamera->Unfollow();
    m_pPlayer->m_canMove = false;

    // add fish ********************************************************************************
    for (int i = 0; i < m_numFish; i++)
    {
        auto fish = std::make_shared<Fish>();
        AddEntity(fish);
        //m_pCamera->Follow(fish->m_transform);
        // add Point lights ********************************************************************************
        AddLight(fish->m_pPointLight);
    }

    // add dir lights ********************************************************************************
    AddLight( // directional light
        std::make_shared <DirLight>(
            glm::vec3(-0.2f, -1.0f, -0.3f), // direction
            glm::vec3(0.5f, 0.5f, 0.5f),    //ambient
            glm::vec3(0.4f, 0.4f, 0.4f),    //diffuse
            glm::vec3(0.5f, 0.5f, 0.5f)     //specular
            ));


#ifdef _DEBUG
    std::cout << "Scene loaded!" << std::endl;
#endif
}

void Scene::AddEntity(std::shared_ptr<Entity> pEntity) {
    m_pSceneEntities.push_back(pEntity);
}

void Scene::Update(const float dt) {

    // switch camera mode
    if (InputSystem::IsKeyPressed(GLFW_KEY_F)) {
        if (m_pCamera->m_freeMode) {
            m_pCamera->Follow(m_pPlayer->m_transform);
            m_pPlayer->m_canMove = true;
            m_pCamera->m_freeMode = false;
        }
        else {
            m_pCamera->Unfollow();
            m_pPlayer->m_canMove = false;
            m_pCamera->m_freeMode = true;
        }
    }

    for (auto pEntity : m_pSceneEntities) {
        pEntity->Update(dt);
    }
}



void Scene::AddLight(std::shared_ptr <DirLight> light) {
    m_DirLights.push_back(light);
}

void Scene::AddLight(std::shared_ptr <PointLight> light) {
    m_PointLights.push_back(light);
}

void Scene::AddLight(std::shared_ptr <SpotLight> light) {
    m_SpotLights.push_back(light);
}

