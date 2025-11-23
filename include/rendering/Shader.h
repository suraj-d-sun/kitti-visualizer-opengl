#pragma once

#include <string>
#include <glm/glm.hpp>

// ------------------------------------------------------------
// Shader
// ------------------------------------------------------------
// Minimal but robust OpenGL shader utility.
// Responsibilities:
//   ✓ Load + compile vertex & fragment shaders
//   ✓ Link shader program
//   ✓ Provide uniform setters (mat4, vec3, float, int, etc.)
//   ✓ Abstract away raw OpenGL calls
//
// Design:
//   - RAII for program lifetime
//   - No file loading here (FileUtils handles that)
//   - No GL includes here (keeps header clean)
// ------------------------------------------------------------

class Shader
{
public:
    Shader();
    ~Shader();

    // Create shader from vertex + fragment shader source strings
    bool compile(const std::string& vertexSrc, const std::string& fragmentSrc);

    // Activate shader program
    void bind() const;

    // Deactivate
    static void unbind();

    // Uniform setters
    void setUniformMat4(const std::string& name, const glm::mat4& value);
    void setUniformVec3(const std::string& name, const glm::vec3& value);
    void setUniformFloat(const std::string& name, float value);
    void setUniformInt(const std::string& name, int value);

    // Check if program is valid
    bool isValid() const { return m_programID != 0; }

private:
    unsigned int m_programID = 0;

    // Internal helpers
    unsigned int compileShader(unsigned int type, const std::string& src);
    int getUniformLocation(const std::string& name);
};
