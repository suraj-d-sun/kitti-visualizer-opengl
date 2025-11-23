// src/rendering/Renderer.cpp
// High-level rendering orchestrator that ties sub-renderers together.
// Reference: /mnt/data/OpenGL_Assignment.pdf

#include "Renderer.h"
#include "PointCloudRenderer.h"
#include "ImageRenderer.h"
#include "TrajectoryRenderer.h"
#include "SteeringWheelRenderer.h"
#include "Camera.h"

#include "utils/Logger.h"

#include <glad/glad.h>

// Constructor / destructor
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::init()
{
    Logger::info("Renderer: initializing sub-renderers...");

    // Create and initialize sub-renderers
    m_pointCloudRenderer = std::make_unique<PointCloudRenderer>();
    m_pointCloudRenderer->initialize();

    m_imageRenderer = std::make_unique<ImageRenderer>();
    m_imageRenderer->initialize();

    m_trajectoryRenderer = std::make_unique<TrajectoryRenderer>();
    m_trajectoryRenderer->initialize();

    m_steeringRenderer = std::make_unique<SteeringWheelRenderer>();
    m_steeringRenderer->initialize();

    Logger::info("Renderer: all sub-renderers initialized.");
    return true;
}

void Renderer::setCamera(Camera* camera)
{
    m_camera = camera;
}

void Renderer::clear()
{
    glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::updateSteeringWheel(const glm::mat4& pose)
{
    // Simple extraction of yaw from pose to approximate steering angle
    // This is an approximation: steering angle isn't directly in pose; depends on vehicle model.
    float yaw = MathUtils::extractYaw(pose);
    if (m_steeringRenderer)
    {
        m_steeringRenderer->setSteeringAngle(yaw);
    }
}

void Renderer::renderFrame(
    Camera& camera,
    const PointCloud& pointCloud,
    const std::vector<unsigned char>& imageData,
    int imageWidth,
    int imageHeight,
    const Trajectory& trajectory)
{
    if (!m_camera)
        m_camera = &camera;

    // Prepare projection & view matrices
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix( (float)800 / 600 ); // you can compute proper aspect

    // Clear buffers
    clear();
    glEnable(GL_DEPTH_TEST);

    // 1) Point cloud
    if (m_pointCloudRenderer)
    {
        m_pointCloudRenderer->uploadPointCloud(pointCloud);
        m_pointCloudRenderer->render(view, projection);
    }

    // 2) Image overlay (draw last so it's on top)
    if (m_imageRenderer && !imageData.empty())
    {
        m_imageRenderer->updateImageTexture(imageWidth, imageHeight, imageData);
        // For image overlay we pass identity view/proj (quad in NDC) or camera matrices depending on shader.
        m_imageRenderer->render(glm::mat4(1.0f), glm::mat4(1.0f));
    }

    // 3) Trajectory
    if (m_trajectoryRenderer)
    {
        m_trajectoryRenderer->uploadTrajectory(trajectory);
        m_trajectoryRenderer->render(view, projection);
    }

    // 4) Steering wheel HUD
    if (m_steeringRenderer)
    {
        m_steeringRenderer->render(view, projection);
    }

    // Swap will be handled by Window class
}
