#pragma once
class StorageBuffer
{
    unsigned int m_RenderID;
public:
    StorageBuffer(const void *data, unsigned int size);
    ~StorageBuffer();
    void Bind();
    void Bind(unsigned int slot);
    void UnBind();
    void Bind4VertexArray();
    void UnBind4VertexArray();
    void GetData(void *data, unsigned int size,unsigned int slot);
};
