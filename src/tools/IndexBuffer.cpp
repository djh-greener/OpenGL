#include "IndexBuffer.h"
#include"myDebug.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count) :m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));
	//m_RendererID绑定为GL_ARRAY_BUFFER类型,同一时刻某种类型只有一个（状态机)
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	//生成帧缓存，写入数据
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * (sizeof(unsigned int)), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind()const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind()const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}