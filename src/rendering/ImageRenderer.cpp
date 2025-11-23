// src/rendering/ImageRenderer.cpp
// Implementation for ImageRenderer
//
// NOTE: Assignment reference: /mnt/data/OpenGL_Assignment.pdf
// (file uploaded in this workspace — you can open it for UI/shader requirements)

#include "ImageRenderer.h"
#include "utils/FileUtils.h"
#include "utils/Logger.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

// Shader file names (relative to your resources/shaders folder)
static const std::string VERT_SHADER_PATH = "resources/shaders/image.vert";
static const std::string FRAG_SHADER_PATH = "resources/shaders/image.frag";

ImageRenderer::ImageRenderer()
    : m_vao(0), m_vbo(0), m_ebo(0), m_textureID(0), m_hasTexture(false)
{
}

ImageRenderer::~ImageRenderer()
{
    if (m_textureID)
        glDeleteTextures(1, &m_textureID);
    if (m_vbo)
        glDeleteBuffers(1, &m_vbo);
    if (m_ebo)
        glDeleteBuffers(1, &m_ebo);
    if (m_vao)
        glDeleteVertexArrays(1, &m_vao);
}

void ImageRenderer::initialize()
{
    Logger::info("ImageRenderer: initializing...");

    // Load shader sources
    std::string vertSrc = FileUtils::readFileAsString(VERT_SHADER_PATH);
    std::string fragSrc = FileUtils::readFileAsString(FRAG_SHADER_PATH);

    if (vertSrc.empty() || fragSrc.empty())
    {
        Logger::error("ImageRenderer: shader source missing. Make sure resources/shaders exist.");
        return;
    }

    if (!m_shader.compile(vertSrc, fragSrc))
    {
        Logger::error("ImageRenderer: Failed to compile shader.");
        return;
    }

    createQuad();
    createTexture();

    Logger::info("ImageRenderer: initialized.");
}

bool ImageRenderer::updateImageTexture(int width, int height, const std::vector<unsigned char>& data)
{
    if (width <= 0 || height <= 0 || data.empty())
    {
        Logger::warn("ImageRenderer::updateImageTexture - invalid image data.");
        return false;
    }

    if (m_textureID == 0)
        createTexture();

    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Upload image data as RGB; if your loader provides 4 channels (RGBA) adjust accordingly
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_hasTexture = true;
    Logger::info("ImageRenderer: texture updated (" + std::to_string(width) + "x" + std::to_string(height) + ").");
    return true;
}

void ImageRenderer::render(const glm::mat4& /*view*/, const glm::mat4& /*projection*/)
{
    if (!m_hasTexture)
        return;

    // Render a screen-aligned quad using the image texture.
    // The shader is expected to sample a 'tex' uniform (sampler2D).
    m_shader.bind();

    // If the shader expects a projection/model/view for positioning, you can set identity.
    glm::mat4 model = glm::mat4(1.0f);
    m_shader.setUniformMat4("u_Model", model);
    m_shader.setUniformMat4("u_View", glm::mat4(1.0f));
    m_shader.setUniformMat4("u_Projection", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    m_shader.setUniformInt("u_Texture", 0);

    glBindVertexArray(m_vao);
    // Draw two triangles (6 indices)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    Shader::unbind();
}

/* -----------------------
   Internal helpers
   ----------------------- */

void ImageRenderer::createQuad()
{
    // Quad vertices: position (x,y), texcoord (u,v)
    // We place quad in NDC so no projection is necessary in simple mode.
    // Vertex format: x, y, u, v
    float vertices[] = {
        // positions   // texcoords
        -1.0f,  1.0f,   0.0f, 1.0f, // top-left
         1.0f,  1.0f,   1.0f, 1.0f, // top-right
         1.0f, -1.0f,   1.0f, 0.0f, // bottom-right
        -1.0f, -1.0f,   0.0f, 0.0f  // bottom-left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute (vec2)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // texcoord attribute (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void ImageRenderer::createTexture()
{
    if (m_textureID != 0)
        return;

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Default texture parameters — can be tuned via config
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Allocate a tiny default 1x1 white texture so the shader has something before real image loads
    unsigned char white[3] = { 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_hasTexture = true;
}
