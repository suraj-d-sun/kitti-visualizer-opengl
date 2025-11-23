#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "IRenderable.h"
#include "Shader.h"

// ------------------------------------------------------------
// ImageRenderer
// ------------------------------------------------------------
// Responsible for:
//   ✓ Rendering 2D camera images as textured quads
//   ✓ Converting raw KITTI image data into OpenGL textures
//   ✓ Handling viewport-aligned rendering
//
// Design:
//   - Uses a simple screen-aligned quad in NDC
//   - Shader handles texture drawing
//   - Texture updates happen dynamically (one per frame)
// ------------------------------------------------------------

class ImageRenderer : public IRenderable
{
public:
    ImageRenderer();
    ~ImageRenderer();

    // Setup quad geometry, load shaders
    void initialize() override;

    // Update texture from raw RGB pixel array
    bool updateImageTexture(int width, int height, const std::vector<unsigned char>& data);

    // Render the textured quad
    void render(const glm::mat4& view,
                const glm::mat4& projection) override;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    unsigned int m_textureID = 0;

    Shader m_shader;

    bool m_hasTexture = false;

    // Internal helpers
    void createQuad();
    void createTexture();
};
