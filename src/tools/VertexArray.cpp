#include "VertexArray.h"
#include"VertexBuffer.h"
#include "StorageBuffer.h"
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
        if(!elements[i].isPadding){
            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void*)offset));
        }
        offset += elements[i].count * VertexBufferElement::getElementTypeSize(elements[i].type);
	}
}

void VertexArray::AddBuffer(StorageBuffer &sb, VertexBufferLayout &layout) {
    Bind();
    sb.Bind4VertexArray();
    int offset = 0;
    auto elements = layout.GetElements();
    int index = 0;
    for (int i = 0; i < elements.size(); i++)
    {
        if(!elements[i].isPadding){
            GLCall(glEnableVertexAttribArray(index));
            GLCall(glVertexAttribPointer(index, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void*)offset));
            index++;
        }
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


