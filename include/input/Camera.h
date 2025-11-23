#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ------------------------------------------------------------
// Camera
// ------------------------------------------------------------
// A standard Orbit / FPS hybrid camera used for 3D navigation.
//
// Responsibilities:
//   ✓ Maintain camera position, direction, up vectors
//   ✓ Produce view & projection matrices
//   ✓ Handle:
//        • rotate
//        • zoom
//        • pan
//        • movement updates
//   ✓ Used by Renderer and InputHandler
//
// No rendering or input code here — SRP.
// ------------------------------------------------------------

class Camera
{
public:
    Camera();
    ~Camera() = default;

    // Updates internal vectors (called every frame or after input)
    void update();

    // View matrix (lookAt)
    glm::mat4 getViewMatrix() const;

    // Projection matrix
    glm::mat4 getProjectionMatrix() const;

    // Mouse look (orbit / rotate)
    void rotate(float deltaX, float deltaY);

    // Zoom in/out
    void zoom(float amount);

    // Pan camera
    void pan(float deltaX, float deltaY);

    // Setters for initial camera configuration
    void setAspectRatio(float aspect);
    void setPosition(const glm::vec3& pos);
    void setTarget(const glm::vec3& target);

private:
    // Computes direction/up/right based on yaw/pitch
    void recalcVectors();

private:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_direction;

    float m_yaw;
    float m_pitch;
    float m_distance;

    // Projection parameters
    float m_fov = 60.0f;
    float m_aspect = 16.0f / 9.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane  = 1000.0f;
};
