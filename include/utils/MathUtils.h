#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace MathUtils
{
    // ------------------------------------------------------------
    // Clamp a value between min and max
    // ------------------------------------------------------------
    template<typename T>
    T clamp(T value, T minVal, T maxVal)
    {
        return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
    }

    // ------------------------------------------------------------
    // Convert Euler angles (degrees) to quaternion
    // ------------------------------------------------------------
    glm::quat eulerToQuat(const glm::vec3& eulerDeg);

    // ------------------------------------------------------------
    // Convert quaternion to rotation matrix
    // ------------------------------------------------------------
    glm::mat4 quatToMatrix(const glm::quat& q);

    // ------------------------------------------------------------
    // Normalize a direction vector safely
    // ------------------------------------------------------------
    glm::vec3 safeNormalize(const glm::vec3& v);

    // ------------------------------------------------------------
    // Compute LookAt matrix
    // (Wrapper around glm::lookAt, for readability & abstraction)
    // ------------------------------------------------------------
    glm::mat4 computeLookAt(
        const glm::vec3& position,
        const glm::vec3& target,
        const glm::vec3& up);

    // ------------------------------------------------------------
    // Linear interpolation between two floats
    // ------------------------------------------------------------
    float lerp(float a, float b, float t);

    // ------------------------------------------------------------
    // Convert 4x4 KITTI pose matrix to position vector
    // (pose[3] column contains translation)
    // ------------------------------------------------------------
    glm::vec3 extractTranslation(const glm::mat4& m);

    // ------------------------------------------------------------
    // Convert KITTI rotation (3x3) to yaw angle (approx.)
    // ------------------------------------------------------------
    float extractYaw(const glm::mat4& pose);
}
