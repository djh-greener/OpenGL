#pragma once
#include"glad/glad.h"
#include<iostream>
#include<string>
using namespace std;
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static string Enum2String(unsigned int ErrorEnum)
{
	string error;
	switch (ErrorEnum)
	{
	case GL_INVALID_ENUM:										error = "INVALID_ENUM"; break;
	case GL_INVALID_VALUE:										error = "INVALID_VALUE"; break;
	case GL_INVALID_OPERATION:							error = "INVALID_OPERATION"; break;
	case GL_OUT_OF_MEMORY:									error = "OUT_OF_MEMORY"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "INVALID_FRAMEBUFFER_OPERATION"; break;
	}
	return error;
}
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error :"
			<< Enum2String(error) << " "
			<< function << " "
			<< line << std::endl;
		return false;
	}
	return true;
}

class Log {
public:
    enum Level {
        Default = 37,
        Warning = 33,
        Error = 31
    };

    template<typename T>
    Log& operator<<(const T& value) {
        std::cout << "\033[" << int(level) << "m" << value << "\033[0m";
        return *this;
    }

    Log& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        std::cout << manipulator;
        level = Level::Default;
        return *this;
    }
    Log& operator<<(Level level) {
        this->level = level;
        return *this;
    }

    static Log& GetInstance()
    {
        static Log Instance;
        return Instance;
    }
private:
    Level level = Level::Default;
    Log() {}
    ~Log() {}
};

