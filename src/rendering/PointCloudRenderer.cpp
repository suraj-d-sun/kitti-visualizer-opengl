// src/rendering/PointCloudRenderer.cpp
// Production-quality point cloud renderer with intensity->heatmap coloring (Option 2).
// Reference: /mnt/data/OpenGL_Assignment.pdf

#include "PointCloudRenderer.h"
#include "PointCloud.h"
#include "utils/FileUtils.h"
#include "utils/Logger.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstring>

static const std::string PC_VERT_SHADER = "resources/shaders/pointcloud.vert";
static const std::string PC_FRAG_SHADER = "resources/shaders/pointcloud.frag";

PointCloudRenderer::PointCloudRenderer()
    : m_vao(0), m_vbo(0), m_pointCount(0), m_isInitialized(false)
{
}

PointCloudRenderer::~PointCloudRenderer()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void PointCloudRenderer::initialize()
{
    Logger::info("PointCloudRenderer: initializing...");

    std::string vertSrc = FileUtils::readFileAsString(PC_VERT_SHADER);
    std::string fragSrc = FileUtils::readFileAsString(PC_FRAG_SHADER);

    if (vertSrc.empty() || fragSrc.empty())
    {
        Logger::error("PointCloudRenderer: shader files missing: " + PC_VERT_SHADER + " or " + PC_FRAG_SHADER);
        return;
    }

    if (!m_shader.compile(vertSrc, fragSrc))
    {
        Logger::error("PointCloudRenderer: shader compile failed.");
        return;
    }

    createBuffers();
    m_isInitialized = true;
    Logger::info("PointCloudRenderer: initialized.");
}

void PointCloudRenderer::createBuffers()
{
    // VBO stores: float x,y,z,intensity  -> 4 floats per vertex
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Initially allocate a small buffer; will use glBufferData with NULL to allocate and glBufferSubData for updates.
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // attribute 0: vec3 position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));

    // attribute 1: float intensity
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void PointCloudRenderer::uploadPointCloud(const PointCloud& cloud)
{
    if (!m_isInitialized)
    {
        Logger::warn("PointCloudRenderer: upload called before initialization. Initializing now.");
        initialize();
        if (!m_isInitialized) return;
    }

    const auto& pts = cloud.getPoints();
    std::size_t n = pts.size();
    m_pointCount = n;
    if (n == 0) return;

    // Create contiguous buffer of floats (x,y,z,intensity)
    std::vector<float> buf;
    buf.reserve(n * 4);
    for (const auto& p : pts)
    {
        buf.push_back(p.x);
        buf.push_back(p.y);
        buf.push_back(p.z);
        // intensity may be stored in p.intensity (0..1). Clamp to [0,1]
        float inten = p.intensity;
        if (inten < 0.0f) inten = 0.0f;
        if (inten > 1.0f) inten = 1.0f;
        buf.push_back(inten);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // If buffer size changed, reallocate; otherwise orphan and update
    GLint currentSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &currentSize);
    GLsizeiptr newSize = static_cast<GLsizeiptr>(buf.size() * sizeof(float));

    if (static_cast<GLsizeiptr>(currentSize) < newSize)
    {
        // allocate new
        glBufferData(GL_ARRAY_BUFFER, newSize, buf.data(), GL_DYNAMIC_DRAW);
    }
    else
    {
        // orphan and upload
        glBufferData(GL_ARRAY_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, newSize, buf.data());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointCloudRenderer::render(const glm::mat4& view, const glm::mat4& projection)
{
    if (!m_isInitialized || m_pointCount == 0)
        return;

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();
    m_shader.setUniformMat4("u_View", view);
    m_shader.setUniformMat4("u_Projection", projection);

    // control point size via uniform if shader supports it
    m_shader.setUniformFloat("u_PointSize", 2.0f);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_pointCount));
    glBindVertexArray(0);

    Shader::unbind();

    glDisable(GL_BLEND);
}
