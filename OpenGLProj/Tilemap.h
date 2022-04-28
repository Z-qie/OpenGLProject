#pragma once

#include <memory>

#include "Entity.h"
#include <GLFW/glfw3.h>

// batch rendering / instancing
class Tilemap : public Entity
{
public:
    // for convinience, we set up everything here
    Tilemap();
    ~Tilemap() {
        delete[] m_modelMatrices; 
        delete[]m_curRippleHeights;
        delete[]m_preRippleHeights;


    };

    bool IsInBound(int x, int y) const;
    int GetMapXAt(int curX) const;
    int GetMapYAt(int curY) const;
    void Ripple(float deltatime);
    virtual void Update(const float deltatime);
    virtual void DrawSelfAndChild(const Camera& camera) override;


public:

    int m_mapSizeX{ 100 };
    int m_mapSizeY{ 100 };
    float m_tileSize{ 0.85f };

    int m_baseOffsetX{ -1 };
    int m_baseOffsetY{ 0 };

    GLuint m_buffer;

    glm::mat4* m_modelMatrices;
    float* m_curRippleHeights;
    float* m_preRippleHeights;

    float m_nextRippleTime{ 0 };
    float m_curTime{ 0 };
    float m_rippleInterval{ 0.03f };

    Transform* m_PlayerTransform{ nullptr };

};