#pragma once
#include<vector>
using namespace std;
#include<glad/glad.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
	//.h里的函数定义必须是静态的，否则多个文件包含的时候重定义错误
	static unsigned int getElementTypeSize(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:					return 4;
		case GL_UNSIGNED_INT:		return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		default:									__debugbreak();
		}
		return 0;
	}
};
class VertexBufferLayout
{
	vector<VertexBufferElement>m_Elements;
	unsigned int m_Stride=0;
public:
	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back(VertexBufferElement({ GL_FLOAT, count, false }));
		m_Stride += count * VertexBufferElement::getElementTypeSize(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_INT, count, false }));
		m_Stride += count * VertexBufferElement::getElementTypeSize(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_BYTE, count, false }));
		m_Stride += count * VertexBufferElement::getElementTypeSize(GL_UNSIGNED_BYTE);
	}

	vector<VertexBufferElement> GetElements()const { return m_Elements; };
	unsigned int GetStride() const{ return m_Stride; };
};

