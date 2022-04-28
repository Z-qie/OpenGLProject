#include <iostream>
#include "Camera.h"
#include "InputSystem.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), CursorSensitivity(SENSITIVITY), Zoom(ZOOM), Near(NEAR), Far(FAR)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

void Camera::Update(const float deltatime) {
    // If the mouse position has changed, recalculate vectors
    if (InputSystem::IsCursorMoved()) {
        ProcessCursorMovement(InputSystem::GetCursorOffsetX(), InputSystem::GetCursorOffsetY(), true);
    }
    // Update Zoom
    ProcessCursorScroll(InputSystem::GetCursorScrollY());

    if (m_freeMode) {
        // Update Keyboard
        ProcessKeyboard(deltatime);
    }
    else { // bind position to target
        Position = m_entityFollowed->GetGlobalPosition() + m_followOffset;
        Position.y = m_followOffset.y;
    }
}

void Camera::Follow(Transform& transform) {
    m_freeMode = false;
    m_entityFollowed = &transform;
    Yaw = YAW;
    Pitch = PITCH;
    Zoom = ZOOM;
}


void Camera::Unfollow() {
    m_freeMode = true;
    Yaw = YAW;
    Pitch = PITCH;
    Zoom = ZOOM;
}


void Camera::ProcessKeyboard(float deltatime)
{
    float velocity = MovementSpeed * deltatime;
    //float velocity = MovementSpeed;
    if (InputSystem::IsKeyPressed(GLFW_KEY_W)) { Position += Front * velocity; }
    if (InputSystem::IsKeyPressed(GLFW_KEY_S)) { Position -= Front * velocity; }
    if (InputSystem::IsKeyPressed(GLFW_KEY_A)) { Position -= Right * velocity; }
    if (InputSystem::IsKeyPressed(GLFW_KEY_D)) { Position += Right * velocity; }
}

void Camera::ProcessCursorMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= CursorSensitivity;
    yoffset *= CursorSensitivity;


    Yaw += xoffset;
    if (!m_freeMode) {
        if (Yaw > 89.0f)
            Yaw = 89.0f;
        if (Yaw < -89.0f)
            Yaw = -89.0f;
    }

    Pitch += yoffset;
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_freeMode) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        else {
            if (Pitch > 50.0f)
                Pitch = 50.0f;
            if (Pitch < -50.0f)
                Pitch = -50.0f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessCursorScroll(float yoffset)
{
    Zoom -= (float)yoffset * 0.05f;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 60.0f)
        Zoom = 60.0f;
}


void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}