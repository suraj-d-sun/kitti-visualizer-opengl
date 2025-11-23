// src/rendering/SteeringWheelRenderer.cpp
// Steering wheel mesh (procedural ring) with rotation based on steering angle.
// Reference: /mnt/data/OpenGL_Assignment.pdf

#include "SteeringWheelRenderer.h"
#include "utils/FileUtils.h"
#include "utils/Logger.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>

static const std::string WHEEL_VERT = "resources/shaders/wheel.vert";
static const std::string WHEEL_FRAG = "resources/shaders/wheel.frag";

SteeringWheelRenderer::SteeringWheelRenderer()
    : m_vao(0), m_vbo(0), m_isInitialized(false), m_steeringAngle(0.0f)
{
}

SteeringWheelRenderer::~SteeringWheelRenderer()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void SteeringWheelRenderer::initialize()
{
    Logger::info("SteeringWheelRenderer: initializing...");

    std::string vs = FileUtils::readFileAsString(WHEEL_VERT);
    std::string fs = FileUtils::readFileAsString(WHEEL_FRAG);

    if (vs.empty() || fs.empty())
    {
        Logger::error("SteeringWheelRenderer: shaders missing.");
        return;
    }

    if (!m_shader.compile(vs, fs))
    {
        Logger::error("SteeringWheelRenderer: shader compile failed.");
        return;
    }

    createWheelGeometry();
    m_isInitialized = true;
    Logger::info("SteeringWheelRenderer: initialized.");
}

void SteeringWheelRenderer::createWheelGeometry()
{
    // Simple ring made of line loop (circle) with inner thickness via two rings triangulated.
    const int segments = 64;
    const float outerR = 0.5f;
    const float innerR = 0.35f;

    std::vector<float> vertices;
    vertices.reserve(segments * 6);

    for (int i = 0; i < segments; ++i)
    {
        float t0 = (2.0f * M_PI * i) / segments;
        float xO = cosf(t0) * outerR;
        float yO = sinf(t0) * outerR;
        float xI = cosf(t0) * innerR;
        float yI = sinf(t0) * innerR;

        // store outer then inner (x,y,z)
        vertices.push_back(xO); vertices.push_back(yO); vertices.push_back(0.0f);
        vertices.push_back(xI); vertices.push_back(yI); vertices.push_back(0.0f);
    }

    // Create VAO/VBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void SteeringWheelRenderer::setSteeringAngle(float angleRadians)
{
    m_steeringAngle = angleRadians;
}

void SteeringWheelRenderer::render(const glm::mat4& view, const glm::mat4& projection)
{
    if (!m_isInitialized) return;

    m_shader.bind();

    // model: rotate wheel around its forward axis and place it in lower-right corner of screen (overlay)
    glm::mat4 model = glm::mat4(1.0f);

    // small transform to make it look like HUD element (translate and scale)
    model = glm::translate(model, glm::vec3(2.0f, -1.2f, -2.5f)); // tweak to suit camera
    model = glm::scale(model, glm::vec3(0.8f));
    model = glm::rotate(model, m_steeringAngle, glm::vec3(0.0f, 0.0f, 1.0f));

    m_shader.setUniformMat4("u_Model", model);
    m_shader.setUniformMat4("u_View", view);
    m_shader.setUniformMat4("u_Projection", projection);

    m_shader.setUniformVec3("u_Color", glm::vec3(0.15f, 0.15f, 0.15f));

    glBindVertexArray(m_vao);

    // Draw segments as pairs: 2 vertices per segment -> draw as GL_TRIANGLE_STRIP or GL_LINES for ring edges.
    // We'll draw as GL_LINES connecting outer->inner pairs for visual ring.
    const int segments = 64;
    glDrawArrays(GL_LINES, 0, segments * 2);

    glBindVertexArray(0);
    Shader::unbind();
}
