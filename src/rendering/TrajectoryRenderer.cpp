// src/rendering/TrajectoryRenderer.cpp
// Efficient trajectory line-strip renderer.
// Reference: /mnt/data/OpenGL_Assignment.pdf

#include "TrajectoryRenderer.h"
#include "utils/FileUtils.h"
#include "utils/Logger.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static const std::string TRAJ_VERT = "resources/shaders/trajectory.vert";
static const std::string TRAJ_FRAG = "resources/shaders/trajectory.frag";

TrajectoryRenderer::TrajectoryRenderer()
    : m_vao(0), m_vbo(0), m_pointCount(0), m_isInitialized(false)
{
}

TrajectoryRenderer::~TrajectoryRenderer()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void TrajectoryRenderer::initialize()
{
    Logger::info("TrajectoryRenderer: initializing...");

    std::string vs = FileUtils::readFileAsString(TRAJ_VERT);
    std::string fs = FileUtils::readFileAsString(TRAJ_FRAG);

    if (vs.empty() || fs.empty())
    {
        Logger::error("TrajectoryRenderer: shader missing.");
        return;
    }

    if (!m_shader.compile(vs, fs))
    {
        Logger::error("TrajectoryRenderer: shader compile failed.");
        return;
    }

    createBuffers();
    m_isInitialized = true;
    Logger::info("TrajectoryRenderer: initialized.");
}

void TrajectoryRenderer::createBuffers()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // start with empty buffer
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0); // position vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void TrajectoryRenderer::uploadTrajectory(const Trajectory& trajectory)
{
    if (!m_isInitialized)
    {
        Logger::warn("TrajectoryRenderer: upload before init -> initializing.");
        initialize();
        if (!m_isInitialized) return;
    }

    const auto& pts = trajectory.getPath();
    m_pointCount = pts.size();
    if (m_pointCount == 0) return;

    std::vector<float> buf;
    buf.reserve(m_pointCount * 3);
    for (const auto& p : pts)
    {
        buf.push_back(p.x);
        buf.push_back(p.y);
        buf.push_back(p.z);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    GLint currentSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &currentSize);
    GLsizeiptr newSize = static_cast<GLsizeiptr>(buf.size() * sizeof(float));
    if (static_cast<GLsizeiptr>(currentSize) < newSize)
    {
        glBufferData(GL_ARRAY_BUFFER, newSize, buf.data(), GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, newSize, buf.data());
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TrajectoryRenderer::render(const glm::mat4& view, const glm::mat4& projection)
{
    if (!m_isInitialized || m_pointCount == 0)
        return;

    glLineWidth(2.0f);
    m_shader.bind();
    m_shader.setUniformMat4("u_View", view);
    m_shader.setUniformMat4("u_Projection", projection);
    m_shader.setUniformVec3("u_Color", glm::vec3(1.0f, 0.8f, 0.0f)); // amber

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_pointCount));
    glBindVertexArray(0);

    Shader::unbind();
}
