#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
	unsigned int m_VertexCount;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const { return m_VertexCount; }
};

#endif