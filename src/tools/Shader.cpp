#include "Shader.h"
#include "MyDebug.h"
#include<glad/glad.h>
#include <fstream>
#include<sstream>
Shader::Shader(const string FilePath):FilePath(FilePath)
{
    ShaderProgramSource sys = ParseShader(FilePath);
    ShaderID = CreateShaderProgram(sys.VertexShader, sys.FragmentShader, sys.GeometryShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(ShaderID))
}

void Shader::Bind()
{
    GLCall(glUseProgram(ShaderID))
}

void Shader::UnBind()
{
    GLCall(glUseProgram(0))
}

void Shader::SetUniform1f(const std::string& name, float f0)
{
    GLCall(glUniform1f(GetUniformLocation(name), f0));
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
    GLCall(glUniform1i(GetUniformLocation(name), i0));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& vec)
{
	GLCall(glUniform2f(GetUniformLocation(name), vec[0], vec[1]));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec)
{
    GLCall(glUniform3f(GetUniformLocation(name), vec[0], vec[1], vec[2]));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec)
{
	GLCall(glUniform4f(GetUniformLocation(name), vec[0], vec[1], vec[2], vec[3]));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{

    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)const
{
    if (m_UniformLocationCach.find(name) != m_UniformLocationCach.end())
        return m_UniformLocationCach[name];
    GLCall(int location = glGetUniformLocation(ShaderID, name.c_str()));
    if (location == -1)
        std::cout << "Warning Uniform: " << name << "doesn't exist!" << std::endl;
    m_UniformLocationCach[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const string FilePath)
{
    enum ShaderType
    {
        None=-1, Vertex=0, Fragment=1, Geometry =2
    };
    fstream fstream(FilePath);
    string line;
	stringstream ss[3];
    ShaderType type = None;
    while (getline(fstream, line))
    {
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
                type = Vertex;
            else if (line.find("fragment") != string::npos)
                type = Fragment;
			else if (line.find("geometry") != string::npos)
				type = Geometry;
        }
        else
            ss[int(type)] << line << "\n";
    }
    return { ss[0].str(), ss[1].str() ,ss[2].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const string ShaderSource)
{
    unsigned int ShaderID = glCreateShader(type);
    const char* src = ShaderSource.c_str();
    glShaderSource(ShaderID, 1, &src, NULL);
    glCompileShader(ShaderID);

    int success;
    char ShaderLog[512];
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ShaderID, 512, NULL, ShaderLog);
        cout << "Shader Compile Failed" << endl;
        cout << "id: " << ShaderID << endl;
        cout << ShaderLog << endl;
    }
    return ShaderID;
}

unsigned int Shader::CreateShaderProgram(const string VertexShaderSource, const string FragmentShaderSource, const string GeometryShaderSource)
{
    unsigned int VertexShader = CompileShader(GL_VERTEX_SHADER, VertexShaderSource);
    unsigned int FragmentShader = CompileShader(GL_FRAGMENT_SHADER, FragmentShaderSource);
	unsigned int GeometryShader;
	if(!GeometryShaderSource.empty())
		GeometryShader = CompileShader(GL_GEOMETRY_SHADER, GeometryShaderSource);

    unsigned int ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
	if (!GeometryShaderSource.empty())
		glAttachShader(ShaderProgram, GeometryShader);

    glLinkProgram(ShaderProgram);

    int success;
    char ShaderLog[512];
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, ShaderLog);
        cout << "Shader Link Failed" << endl;
        cout << "id: " << ShaderID << endl;
        cout << ShaderLog << endl;
    }
    
    return ShaderProgram;
}
