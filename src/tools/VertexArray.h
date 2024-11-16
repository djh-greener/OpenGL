#pragma once
class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
	unsigned int m_RenderID;
public:
	VertexArray();
	~VertexArray();
	//必须要传引用
	void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);

	void Bind();
	void UnBind();
};

