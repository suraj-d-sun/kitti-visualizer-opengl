#pragma once

#include <glm/glm.hpp>

// ------------------------------------------------------------
// IRenderable
// ------------------------------------------------------------
// Interface for all renderable components in the engine.
//
// Responsibilities:
//   ✓ Provide a common API for initialization
//   ✓ Provide a unified render() function
//   ✓ Allow updating of GPU buffers
//
// Implemented by:
//   • PointCloudRenderer
//   • ImageRenderer
//   • SteeringWheelRenderer
//   • TrajectoryRenderer
//
// This interface enables the Renderer to handle all
// renderable systems generically (OCP-friendly).
// ------------------------------------------------------------

class IRenderable
{
public:
    virtual ~IRenderable() = default;

    // Initialize OpenGL buffers, shaders, etc.
    virtual bool init() = 0;

    // Update GPU buffers, textures, or transforms
    virtual void update() = 0;

    // Render the object using view + projection matrices
    virtual void render(const glm::mat4& view,
                        const glm::mat4& projection) = 0;
};
