// src/rendering/Shader.cpp
// Robust shader utility (compile/link, uniforms, RAII).
// Reference: /mnt/data/OpenGL_Assignment.pdf

#include "Shader.h"
#include "utils/Logger.h"
#include "utils/FileUtils.h"

#include <glad/glad.h>
#include <iostream>

Shader::Shader()
    : m_programID(0)
{
}

Shader::~Shader()
{
    if (m_programID != 0)
        glDeleteProgram(m_programID);
}

unsigned int Shader::compileShader(unsigned int type, const std::string& src)
{
    unsigned int id = glCreateShader(type);
    const char* srcc = src.c_str();
    glShaderSource(id, 1, &srcc, nullptr);
    glCompileShader(id);

    // Check
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int logLen = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
        std::string info(logLen, ' ');
        glGetShaderInfoLog(id, logLen, nullptr, &info[0]);
        Logger::error(std::string("Shader compile error: ") + info);
        glDeleteShader(id);
        return 0;
    }
    return id;
}

bool Shader::compile(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (vs == 0) return false;
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (fs == 0) { glDeleteShader(vs); return false; }

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    int success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        int logLen = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
        std::string info(logLen, ' ');
        glGetProgramInfoLog(prog, logLen, nullptr, &info[0]);
        Logger::error(std::string("Shader link error: ") + info);
        glDeleteProgram(prog);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    // Cleanup shaders after linking
    glDetachShader(prog, vs);
    glDetachShader(prog, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Replace program
    if (m_programID != 0)
        glDeleteProgram(m_programID);
    m_programID = prog;
    return true;
}

void Shader::bind() const
{
    if (m_programID != 0)
        glUseProgram(m_programID);
}

void Shader::unbind()
{
    glUseProgram(0);
}

int Shader::getUniformLocation(const std::string& name)
{
    if (m_programID == 0) return -1;
    int loc = glGetUniformLocation(m_programID, name.c_str());
    if (loc < 0)
    {
        // optionally warn only in debug
        // Logger::debug("Uniform not found: " + name);
    }
    return loc;
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& value)
{
    int loc = getUniformLocation(name);
    if (loc >= 0)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value)
{
    int loc = getUniformLocation(name);
    if (loc >= 0)
        glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setUniformFloat(const std::string& name, float value)
{
    int loc = getUniformLocation(name);
    if (loc >= 0)
        glUniform1f(loc, value);
}

void Shader::setUniformInt(const std::string& name, int value)
{
    int loc = getUniformLocation(name);
    if (loc >= 0)
        glUniform1i(loc, value);
}
