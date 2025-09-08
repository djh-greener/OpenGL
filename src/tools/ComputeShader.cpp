#include "ComputeShader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

ComputeShader::ComputeShader(const std::string& filePath) : FilePath(filePath) {
    std::string source = ParseShader(filePath);
    ShaderID = CompileShader(source);
}

ComputeShader::~ComputeShader() {
    glDeleteProgram(ShaderID);
}

void ComputeShader::Bind() {
    glUseProgram(ShaderID);
}

void ComputeShader::UnBind() {
    glUseProgram(0);
}

void ComputeShader::Dispatch(unsigned int x, unsigned int y, unsigned int z) {
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

// Uniform 变量设置接口实现
void ComputeShader::SetUniform1f(const std::string& name, float f0) {
    glUniform1f(GetUniformLocation(name), f0);
}

void ComputeShader::SetUniform1i(const std::string& name, int i0) {
    glUniform1i(GetUniformLocation(name), i0);
}

void ComputeShader::SetUniform2f(const std::string& name, const glm::vec2& vec) {
    glUniform2f(GetUniformLocation(name), vec.x, vec.y);
}

void ComputeShader::SetUniform3f(const std::string& name, const glm::vec3& vec) {
    glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void ComputeShader::SetUniform4f(const std::string& name, const glm::vec4& vec) {
    glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void ComputeShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void ComputeShader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int ComputeShader::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(ShaderID, name.c_str());
    if (location == -1)
        std::cout << "Warning: Uniform " << name << " not found!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

unsigned int ComputeShader::CompileShader(const std::string& source) {
    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Compute Shader Compilation Failed:\n" << infoLog << std::endl;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Compute Shader Linking Failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(shader);
    return program;
}

std::string ComputeShader::ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);
    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
}