#ifndef VERTEX_BUFFER_LAYOUT
#define VERTEX_BUFFER_LAYOUT

#include <vector>
#include <iostream>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		assert(false);
		return 0;
	}
};

namespace Push
{
	template <typename T>
	inline unsigned int Push(unsigned int count, std::vector<VertexBufferElement> &m_Elements)
	{
		//static_assert(false);
		std::cout << "Deu ruim" << std::endl;
		return 0;
	}

	template <>
	inline unsigned int Push<float>(unsigned int count, std::vector<VertexBufferElement> &m_Elements)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		return count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template <>
	inline unsigned int Push<unsigned int>(unsigned int count, std::vector<VertexBufferElement> &m_Elements)
	{
		m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
		return count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	inline unsigned int Push<unsigned char>(unsigned int count, std::vector<VertexBufferElement> &m_Elements)
	{
		m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		return count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
} // namespace Push

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

	template <typename T>
	inline void Push(unsigned int count)
	{
		m_Stride += Push::Push<T>(count, m_Elements);
	}
};

#endif