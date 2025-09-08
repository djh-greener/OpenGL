#pragma once
#include"glm/glm.hpp"

#include<iostream>
#include<string>
#include<unordered_map>
using namespace std;

struct ShaderProgramSource
{
	const string VertexShader;
	const string FragmentShader;
	const string GeometryShader;
};

class Shader
{
	const string FilePath;
	
	unsigned int ShaderID;
	mutable unordered_map<string,int>m_UniformLocationCach;
public:
	Shader(const string FilePath);
	~Shader();
	void Bind();
	void UnBind();

	void SetUniform1f(const std::string& name, float f0);
	void SetUniform1i(const std::string& name, int i0);
	void SetUniform2f(const std::string& name, const glm::vec2& vec);

	void SetUniform3f(const std::string& name, const glm::vec3& vec);
	void SetUniform4f(const std::string& name, const glm::vec4& vec);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
private:
	int GetUniformLocation(const std::string& name)const;
	ShaderProgramSource ParseShader(const string FilePath);
	unsigned int CompileShader(unsigned int  type, const string ShaderSource);
	unsigned int CreateShaderProgram(const string VertexShaderSource, const string FragmentShaderSource,
                                     const string GeometryShaderSource);
};

