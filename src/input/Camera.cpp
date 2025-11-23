#include "Camera.h"
#include "MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& target)
    : m_position(position),
      m_target(target),
      m_up(0.0f, 1.0f, 0.0f),
      m_yaw(-90.0f),
      m_pitch(0.0f),
      m_sensitivity(0.1f),
      m_zoom(45.0f)
{
    updateDirectionVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(m_zoom), aspectRatio, 0.1f, 500.0f);
}

void Camera::moveForward(float delta)
{
    m_position += m_front * delta;
}

void Camera::moveBackward(float delta)
{
    m_position -= m_front * delta;
}

void Camera::moveLeft(float delta)
{
    m_position -= m_right * delta;
}

void Camera::moveRight(float delta)
{
    m_position += m_right * delta;
}

void Camera::moveUp(float delta)
{
    m_position += m_up * delta;
}

void Camera::moveDown(float delta)
{
    m_position -= m_up * delta;
}

void Camera::rotate(float offsetX, float offsetY)
{
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;

    m_yaw   += offsetX;
    m_pitch += offsetY;

    // Constrain pitch to avoid gimbal lock
    m_pitch = MathUtils::clamp(m_pitch, -89.0f, 89.0f);

    updateDirectionVectors();
}

void Camera::zoom(float yOffset)
{
    m_zoom -= yOffset;
    m_zoom = MathUtils::clamp(m_zoom, 10.0f, 90.0f);
}

void Camera::updateDirectionVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::setPosition(const glm::vec3& pos)
{
    m_position = pos;
}

glm::vec3 Camera::getPosition() const
{
    return m_position;
}

glm::vec3 Camera::getFront() const
{
    return m_front;
}

glm::vec3 Camera::getUp() const
{
    return m_up;
}
