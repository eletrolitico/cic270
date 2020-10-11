#ifndef RENDERER
#define RENDERER

#include <GL/glew.h>
#include <signal.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) \
	if (!(x))     \
		raise(SIGTRAP);
#define GLCall(x)   \
	GlClearError(); \
	x;              \
	ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char *function, const char *file, int line);

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