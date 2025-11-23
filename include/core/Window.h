#pragma once

#include <string>
#include <memory>

// Forward-declare GLFW window to avoid including GLFW here
struct GLFWwindow;

class Window
{
public:
    Window();
    ~Window();

    // Creates the GLFW window and initializes OpenGL context
    bool create(int width, int height, const std::string& title);

    // Process pending OS events
    void pollEvents();

    // Swap buffers after rendering a frame
    void swapBuffers();

    // Check if the user requested to close the window
    bool shouldClose() const;

    // Access raw GLFW window pointer
    GLFWwindow* getGLFWwindow() const { return m_window; }

    // Update title dynamically (fps, frameID, etc.)
    void setTitle(const std::string& title);

private:
    bool initGLFW();
    bool initGLAD();

private:
    GLFWwindow* m_window = nullptr;

    int m_width  = 1280;
    int m_height = 720;
    std::string m_title = "KITTI Visualizer";
};
