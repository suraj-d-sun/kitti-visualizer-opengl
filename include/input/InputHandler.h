#pragma once

#include <unordered_map>

class Window;
class Camera;

// ------------------------------------------------------------
// InputHandler
// ------------------------------------------------------------
// Responsible for processing:
//   ✓ Keyboard events
//   ✓ Mouse movement
//   ✓ Mouse scroll (zoom)
//   ✓ Frame navigation keys (next/prev)
//
// This class does NOT handle rendering or camera math.
// It simply detects input events and forwards actions
// to the Camera or Application.
//
// Follows SRP and the Dependency Inversion Principle.
// ------------------------------------------------------------

class InputHandler
{
public:
    InputHandler();
    ~InputHandler() = default;

    // Connect window + initialize GLFW callbacks
    void attachWindow(Window* window);

    // Connect a camera to manipulate
    void attachCamera(Camera* camera);

    // Called every frame — polls input and updates state
    void processInput();

    // Query: did user request next/previous frame?
    bool nextFrameRequested() const { return m_nextFrame; }
    bool prevFrameRequested() const { return m_prevFrame; }

    // Reset after processed
    void clearFrameRequests()
    {
        m_nextFrame = false;
        m_prevFrame = false;
    }

private:
    // Key callback adapter (GLFW → InputHandler)
    static void keyCallbackAdapter(void* handlerPtr,
                                   int key, int scancode, int action, int mods);

    // Mouse movement callback
    static void mouseMoveCallbackAdapter(void* handlerPtr,
                                         double xpos, double ypos);

    // Mouse scroll callback (zoom)
    static void scrollCallbackAdapter(void* handlerPtr,
                                      double xoffset, double yoffset);

private:
    Window* m_window = nullptr;
    Camera* m_camera = nullptr;

    // Mouse tracking
    bool   m_firstMouse = true;
    double m_lastX = 0.0;
    double m_lastY = 0.0;

    // Frame navigation
    bool m_nextFrame = false;
    bool m_prevFrame = false;

    // Keyboard state
    std::unordered_map<int, bool> m_keyState;
};
