#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "IRenderable.h"
#include "Shader.h"

class PointCloud;

// ------------------------------------------------------------
// PointCloudRenderer
// ------------------------------------------------------------
// Responsible for:
//   ✓ Uploading PointCloud data (xyz + intensity) to GPU
//   ✓ Maintaining VAO/VBO buffers for point rendering
//   ✓ Rendering point clouds using GL_POINTS
//
// Features:
//   - Supports intensity-based coloring
//   - Efficient GPU buffer updates when new frames arrive
//   - Works directly with PointCloud objects
//
// SRP & Clean Architecture:
//   - This class ONLY renders.
//   - PointCloud parsing + loading is handled by other modules.
// ------------------------------------------------------------

class PointCloudRenderer : public IRenderable
{
public:
    PointCloudRenderer();
    ~PointCloudRenderer();

    // Setup shader, VAO/VBO
    void initialize() override;

    // Upload new point cloud to GPU
    void uploadPointCloud(const PointCloud& cloud);

    // Render points
    void render(const glm::mat4& view,
                const glm::mat4& projection) override;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;

    Shader m_shader;

    std::size_t m_pointCount = 0;

    bool m_isInitialized = false;

    // internal helpers
    void createBuffers();
};
