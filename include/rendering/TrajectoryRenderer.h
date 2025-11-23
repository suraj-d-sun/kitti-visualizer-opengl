#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "IRenderable.h"
#include "Shader.h"
#include "Trajectory.h"

// ------------------------------------------------------------
// TrajectoryRenderer
// ------------------------------------------------------------
// Responsible for rendering the driven trajectory as a 3D line-strip.
//
// Features:
//   ✓ Efficient line rendering using VAO/VBO
//   ✓ Updates GPU buffer when trajectory grows
//   ✓ Uses a simple colored line (configurable in shader)
//   ✓ Works directly with Trajectory (list of glm::vec3 points)
//
// Notes:
//   - Does NOT modify trajectory data.
//   - Only converts it to GPU buffer + renders.
// ------------------------------------------------------------

class TrajectoryRenderer : public IRenderable
{
public:
    TrajectoryRenderer();
    ~TrajectoryRenderer();

    // Create shader + buffers
    void initialize() override;

    // Update GPU buffer when trajectory changes
    void uploadTrajectory(const Trajectory& trajectory);

    // Draw the line-strip
    void render(const glm::mat4& view,
                const glm::mat4& projection) override;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;

    Shader m_shader;

    std::size_t m_pointCount = 0;

    bool m_isInitialized = false;

    void createBuffers();
};
    