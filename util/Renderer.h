#ifndef RENDERER
#define RENDERER

#include <GL/glew.h>
#include <signal.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	//Renderer();
	//~Renderer();
	void Clear() const;
	void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
	void Draw(const VertexArray &va, const Shader &shader) const;
};

#endif