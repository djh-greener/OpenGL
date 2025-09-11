#pragma once
class VertexBuffer;
class VertexBufferLayout;
class StorageBuffer;
class VertexArray
{
	unsigned int m_RenderID;
public:
	VertexArray();
	~VertexArray();
	//必须要传引用
	void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);
    void AddBuffer(StorageBuffer& sb, VertexBufferLayout& layout);

	void Bind();
	void UnBind();
};

