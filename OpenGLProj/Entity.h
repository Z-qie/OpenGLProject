#pragma once

#include <list>
#include <memory> //std::unique_ptr

#include "Model.h"
#include "Transform.h";
#include "Camera.h"

class Entity {
public:
    Entity() {};
    explicit Entity(std::shared_ptr<Shader>& pShader, std::shared_ptr<Model>& pModel) :m_pShader(pShader), m_pModel(pModel)
    {
        //GLuint matricesIndex = glGetUniformBlockIndex(m_pShader->ID, "Matrices");
        //glUniformBlockBinding(m_pShader->ID, matricesIndex, 0); // hard coded
    };

    // Update
    virtual void Update(const float deltatime);
    // Update transform if it was changed
    void UpdateSelfAndChild();
    // Force update of transform even if local space don't change
    void ForceUpdateSelfAndChild();

    // Scene graph
    template<typename... TArgs>
    void AddChild(TArgs&... args);
    bool HasParent() { return m_pParent != nullptr; }

    // Render
    virtual void DrawSelfAndChild(const Camera& camera);


public:
    Transform m_transform;

    std::shared_ptr<Model> m_pModel{ nullptr };
    std::shared_ptr<Shader> m_pShader{ nullptr };

protected:
    // scene graph
    Entity* m_pParent{ nullptr };
    std::list<std::unique_ptr<Entity>> children;


};