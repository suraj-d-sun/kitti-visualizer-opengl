#pragma once

#include <memory>
#include <glm/glm.hpp>

class Camera;
class PointCloud;
class Trajectory;
class ImageRenderer;
class PointCloudRenderer;
class SteeringWheelRenderer;
class TrajectoryRenderer;

// ------------------------------------------------------------
// Renderer
// ------------------------------------------------------------
// Central rendering orchestrator.
// Responsibilities:
//   ✓ Initialize all renderable subsystems
//   ✓ Manage viewport, clearing, buffer swapping
//   ✓ Render full frame in correct order:
//        1. point cloud
//        2. camera image (texture quad)
//        3. steering wheel indicator
//        4. trajectory path
//
//   ✓ Store camera pointer
//   ✓ Provide renderFrame() to Application
//
// Follows:
//   - SRP (single rendering coordinator)
//   - OCP (easily add new renderables)
//   - DIP (depends only on interfaces/forward declarations)
// ------------------------------------------------------------

class Renderer
{
public:
    Renderer();
    ~Renderer() = default;

    // Initialize OpenGL backend and render systems
    bool init();

    // Set the global camera (used for view/projection)
    void setCamera(Camera* camera);

    // Clear color & depth buffers
    void clear();

    // Render the full scene (called once per frame)
    void renderFrame(const PointCloud& pointCloud,
                     int imageWidth,
                     int imageHeight,
                     const std::vector<unsigned char>& imageData,
                     const Trajectory& trajectory);

private:
    Camera* m_camera = nullptr;

    // Sub-renderers
    std::unique_ptr<PointCloudRenderer>   m_pointCloudRenderer;
    std::unique_ptr<ImageRenderer>        m_imageRenderer;
    std::unique_ptr<SteeringWheelRenderer> m_steeringRenderer;
    std::unique_ptr<TrajectoryRenderer>   m_trajectoryRenderer;
};
