#include "Application.h"
#include "Logger.h"
#include "FileUtils.h"
#include "Config.h"

#include <GLFW/glfw3.h>

Application::Application()
    : m_window(nullptr),
      m_loader(nullptr),
      m_renderer(nullptr),
      m_inputHandler(nullptr),
      m_camera(nullptr),
      m_trajectory(nullptr),
      m_currentFrame(0)
{
}

Application::~Application()
{
    cleanup();
}

bool Application::initialize(const std::string& configPath)
{
    Logger::info("Application initializing...");

    // ------------------------------------------------------------
    // Load settings.ini
    // ------------------------------------------------------------
    m_config = std::make_unique<Config>(configPath);
    if (!m_config->load())
    {
        Logger::warn("Using default configuration values.");
    }

    // ------------------------------------------------------------
    // Window creation
    // ------------------------------------------------------------
    int width  = m_config->getInt("window_width", 1280);
    int height = m_config->getInt("window_height", 720);

    std::string title = m_config->getString("window_title", "KITTI Visualizer");

    m_window = std::make_unique<Window>(width, height, title);
    if (!m_window->initialize())
        return false;

    // ------------------------------------------------------------
    // Camera
    // ------------------------------------------------------------
    m_camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 5.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    // ------------------------------------------------------------
    // Input handler
    // ------------------------------------------------------------
    m_inputHandler = std::make_unique<InputHandler>();
    m_inputHandler->attachWindow(m_window->getNativeHandle());
    m_inputHandler->attachCamera(m_camera.get());

    // ------------------------------------------------------------
    // KITTI loader
    // ------------------------------------------------------------
    std::string seqPath = m_config->getString("sequence_path", "data/kitti/sequences/00");

    Logger::info("Using KITTI sequence: " + seqPath);

    m_loader = std::make_unique<KittiDataLoader>(seqPath);
    if (!m_loader->initialize())
    {
        Logger::error("Failed to load KITTI sequence.");
        return false;
    }

    // ------------------------------------------------------------
    // Trajectory
    // ------------------------------------------------------------
    m_trajectory = std::make_unique<Trajectory>();

    // ------------------------------------------------------------
    // Renderer
    // ------------------------------------------------------------
    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->initialize())
    {
        Logger::error("Renderer failed to initialize.");
        return false;
    }

    Logger::info("Application initialized successfully.");
    return true;
}

void Application::run()
{
    Logger::info("Starting main loop...");

    while (!m_window->shouldClose())
    {
        // -------------------------------
        // Handle input
        // -------------------------------
        m_inputHandler->update();

        // Frame navigation
        if (m_inputHandler->nextFrameRequested())
            nextFrame();
        else if (m_inputHandler->prevFrameRequested())
            prevFrame();

        // -------------------------------
        // Load data for current frame
        // -------------------------------
        glm::mat4 pose = m_loader->loadPose(m_currentFrame);
        PointCloud cloud = m_loader->loadPointCloud(m_currentFrame);

        int imgW, imgH;
        std::vector<unsigned char> imageData;
        m_loader->loadImage(m_currentFrame, imgW, imgH, imageData);

        // Update trajectory
        glm::vec3 pos = MathUtils::extractTranslation(pose);
        m_trajectory->addPoint(pos);

        // Update steering wheel orientation
        m_renderer->updateSteeringWheel(pose);

        // -------------------------------
        // Render everything
        // -------------------------------
        m_renderer->renderFrame(
            *m_camera,
            cloud,
            imageData, imgW, imgH,
            *m_trajectory
        );

        // -------------------------------
        // Swap buffers
        // -------------------------------
        m_window->update();
    }
}

void Application::cleanup()
{
    Logger::info("Cleaning up application...");

    m_renderer.reset();
    m_loader.reset();
    m_inputHandler.reset();
    m_camera.reset();
    m_window.reset();
    m_trajectory.reset();
}

void Application::nextFrame()
{
    if (m_currentFrame + 1 < m_loader->getTotalFrames())
    {
        m_currentFrame++;
        Logger::debug("Next frame: " + std::to_string(m_currentFrame));
    }
}

void Application::prevFrame()
{
    if (m_currentFrame > 0)
    {
        m_currentFrame--;
        Logger::debug("Prev frame: " + std::to_string(m_currentFrame));
    }
}
