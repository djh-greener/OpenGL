#pragma once
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

class ComputeShader {
    const std::string FilePath;
    unsigned int ShaderID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    ComputeShader(const std::string& filePath);
    ~ComputeShader();

    void Bind();
    void UnBind();

    void Dispatch(unsigned int x, unsigned int y, unsigned int z);

    // Uniform 变量设置接口
    void SetUniform1f(const std::string& name, float f0);
    void SetUniform1i(const std::string& name, int i0);
    void SetUniform2f(const std::string& name, const glm::vec2& vec);
    void SetUniform3f(const std::string& name, const glm::vec3& vec);
    void SetUniform4f(const std::string& name, const glm::vec4& vec);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);

private:
    unsigned int CompileShader(const std::string& source);
    std::string ParseShader(const std::string& filePath);
    int GetUniformLocation(const std::string& name) const;
};