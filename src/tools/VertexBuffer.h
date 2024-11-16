#pragma once
class VertexBuffer
{
	unsigned int m_RenderID;
public:
	VertexBuffer(const void *data, unsigned int size);
	~VertexBuffer();
	void Bind();
	void UnBind();
};

