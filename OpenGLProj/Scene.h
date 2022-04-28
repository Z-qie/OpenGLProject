#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "Player.h"
#include "Tilemap.h"
#include "Light.h"


class Scene {
    friend class Engine;
    friend class RenderSystem;
public:
    virtual void Init(Camera& camera);
    virtual void Update(const float dt);


protected:
    void AddLight(std::shared_ptr < DirLight> light);
    void AddLight(std::shared_ptr < PointLight> light);
    void AddLight(std::shared_ptr < SpotLight> light);

    void AddEntity(std::shared_ptr<Entity> v);

private:

    std::vector<std::shared_ptr<DirLight>> m_DirLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>> m_SpotLights;

    Camera* m_pCamera{ nullptr };
    std::shared_ptr<Player> m_pPlayer;
    std::shared_ptr<Tilemap> m_pTilemap; // instansing
    std::shared_ptr<Entity> m_pTest; // instansing


    std::vector<std::shared_ptr<Entity>> m_pSceneEntities;
    int m_numFish{ 30 };
};