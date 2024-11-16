#pragma once
class UniformBuffer
{
	unsigned int m_RenderID;
	unsigned int m_Point;
public:
	UniformBuffer(unsigned int point, unsigned int size);
	void Bind();
	void UnBind();
	void AddBuffer(int start, int length, void* data);
};

