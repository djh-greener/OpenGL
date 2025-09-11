#include "StorageBuffer.h"
#include"MyDebug.h"
StorageBuffer::StorageBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RenderID));
    Bind();
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW));
    UnBind();
}

StorageBuffer::~StorageBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RenderID));
}

void StorageBuffer::Bind()
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderID));
}

void StorageBuffer::Bind(unsigned int slot) {
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RenderID));
}

void StorageBuffer::UnBind()
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void StorageBuffer::Bind4VertexArray() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}

void StorageBuffer::UnBind4VertexArray() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void StorageBuffer::GetData(void *data, unsigned int size, unsigned int slot) {
    GLCall(glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, slot, size, data));
}


