#pragma once

#include <glm/glm.hpp>
#include "IRenderable.h"
#include "Shader.h"

// ------------------------------------------------------------
// SteeringWheelRenderer
// ------------------------------------------------------------
// Renders a 3D steering wheel model and rotates it according to
// the vehicle's yaw/steering angle.
//
// Features:
//   ✓ Loads wheel geometry (simple mesh or procedural circle)
//   ✓ Maintains VAO/VBO
//   ✓ Applies rotation transform based on current steering angle
//   ✓ Uses its own shader pipeline
//
// Note:
//   - Does NOT interpret vehicle dynamics.
//   - Application passes steering angle (in radians).
// ------------------------------------------------------------

class SteeringWheelRenderer : public IRenderable
{
public:
    SteeringWheelRenderer();
    ~SteeringWheelRenderer();

    // Initialize geometry, buffers, and shader
    void initialize() override;

    // Update steering rotation (called every frame)
    void setSteeringAngle(float angleRadians);

    // Render the wheel
    void render(const glm::mat4& view,
                const glm::mat4& projection) override;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;

    Shader m_shader;

    float m_steeringAngle = 0.0f;   // rotation about Z-axis or Y-axis depending on model

    bool m_isInitialized = false;

    // Internal: build wheel mesh (e.g., circle or loaded model)
    void createWheelGeometry();
};
