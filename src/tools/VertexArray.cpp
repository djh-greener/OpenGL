#include "VertexArray.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include"MyDebug.h"
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RenderID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	int offset = 0;
	auto elements = layout.GetElements();
	for (int i = 0; i < elements.size(); i++) 
	{
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void*)offset));
		offset += elements[i].count * VertexBufferElement::getElementTypeSize(elements[i].type);
	}
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(m_RenderID));
}

void VertexArray::UnBind()
{
	GLCall(glBindVertexArray(0));
}
