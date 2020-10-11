#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	VertexBuffer(const void *data, unsigned int count);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
};

#endif