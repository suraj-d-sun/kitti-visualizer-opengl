#include "Window.h"
#include "Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr)
{
}

Window::~Window()
{
    shutdown();
}

bool Window::initialize()
{
    Logger::info("Initializing GLFW window...");

    if (!glfwInit())
    {
        Logger::error("Failed to initialize GLFW.");
        return false;
    }

    // OpenGL version 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        Logger::error("Failed to create GLFW window.");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    // Load OpenGL function pointers via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::error("Failed to initialize GLAD.");
        return false;
    }

    Logger::info("GLFW window and GLAD initialized successfully.");

    // Initial viewport
    glViewport(0, 0, m_width, m_height);

    // Register resize callback
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window,
        [](GLFWwindow* win, int w, int h)
        {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
            if (self)
            {
                self->m_width = w;
                self->m_height = h;
                glViewport(0, 0, w, h);
            }
        });

    return true;
}

void Window::update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::shutdown()
{
    if (m_window)
    {
        Logger::info("Destroying GLFW window...");
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

GLFWwindow* Window::getNativeHandle() const
{
    return m_window;
}

int Window::getWidth() const { return m_width; }
int Window::getHeight() const { return m_height; }
