#include "InputHandler.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

InputHandler::InputHandler()
    : m_window(nullptr),
      m_camera(nullptr),
      m_firstMouse(true),
      m_lastX(0.0f),
      m_lastY(0.0f),
      m_nextFrame(false),
      m_prevFrame(false)
{
}

void InputHandler::attachWindow(GLFWwindow* window)
{
    m_window = window;
}

void InputHandler::attachCamera(Camera* camera)
{
    m_camera = camera;
}

void InputHandler::update()
{
    if (!m_window)
        return;

    handleKeyboard();
    handleMouseMovement();
    handleMouseScroll();

    // Reset navigation flags every frame
    m_nextFrame = false;
    m_prevFrame = false;

    // Frame navigation
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_nextFrame = true;

    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_prevFrame = true;
}

void InputHandler::handleKeyboard()
{
    if (!m_camera)
        return;

    float speed = 0.3f;

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->moveForward(speed);

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->moveBackward(speed);

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->moveLeft(speed);

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->moveRight(speed);

    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
        m_camera->moveDown(speed);

    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
        m_camera->moveUp(speed);
}

void InputHandler::handleMouseMovement()
{
    if (!m_camera)
        return;

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    if (m_firstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
        return;
    }

    float offsetX = static_cast<float>(xpos - m_lastX);
    float offsetY = static_cast<float>(m_lastY - ypos); // reversed Y

    m_lastX = xpos;
    m_lastY = ypos;

    // Rotate camera
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
    {
        m_camera->rotate(offsetX, offsetY);
    }
}

void InputHandler::handleMouseScroll()
{

}

bool InputHandler::nextFrameRequested() const
{
    return m_nextFrame;
}

bool InputHandler::prevFrameRequested() const
{
    return m_prevFrame;
}
