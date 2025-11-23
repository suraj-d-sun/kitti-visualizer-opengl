#pragma once

#include <memory>
#include <string>
#include <vector>

class Window;
class Renderer;
class InputHandler;
class Camera;
class IKittiLoader;
class Trajectory;
class SteeringWheelRenderer;

class Application
{
public:
    Application();
    ~Application();

    bool init(const std::string& configPath);
    void run();
    void shutdown();

private:
    bool loadSettings(const std::string& configPath);
    bool setupWindow();
    bool setupCoreSystems();
    bool setupRenderers();
    bool setupInput();

    void updateFrame();
    void updateSimulation();
    void render();

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<InputHandler> m_inputHandler;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<IKittiLoader> m_dataLoader;
    std::unique_ptr<Trajectory> m_trajectory;
    std::unique_ptr<SteeringWheelRenderer> m_wheelRenderer;

    int m_currentFrame = 0;
    int m_totalFrames  = 0;

    std::string m_datasetPath;
};
