#pragma once

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>

#include "utils.h"
#include "Transform.h"


//Default camera values
const float YAW = 0.0f;
const float PITCH = -30.0f;
const float SPEED = 15.0f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 60.0f; // the higher, the smaller
const float NEAR = 0.1f;
const float FAR = 300.f;

//An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
// free look & over-shoulder
class Camera
{
public:
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(-50.f, 60.f, 40.f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);


    void Update(const float deltatime);
    void Follow(Transform& transform);
    void Unfollow();

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessCursorMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessCursorScroll(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float CursorSensitivity;
    float Zoom;
    float Near;
    float Far;
    bool m_freeMode{ true };

private:
    Transform* m_entityFollowed{ nullptr };
    glm::vec3 m_followOffset{ -30.0f, 22.f, 0.0f };
};
