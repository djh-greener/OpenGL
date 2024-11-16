#include "UniformBuffer.h"
#include <glm/glm.hpp>

#include"MyDebug.h"
//声明Buffer的绑定点和大小
UniformBuffer::UniformBuffer(unsigned int point, unsigned int size):m_Point(point)
{
	GLCall(glGenBuffers(1, &m_RenderID));
	Bind();
	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW));
	UnBind();
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, point, m_RenderID));
}

void UniformBuffer::Bind()
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RenderID));
}

void UniformBuffer::UnBind()
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::AddBuffer(int start, int length, void* data)
{
	Bind();
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, start, length, data));
	UnBind();
}
