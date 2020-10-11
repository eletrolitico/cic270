#include"Renderer.h"
#include<iostream>

void GlClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line) {
	bool res = true;
	while (GLenum err = glGetError()) {
		std::cout << "[OpenGL Error] (" << err << "): " << function << " " << file << ":" << line << std::endl;
		res = false;
	}
	return res;
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader) const {
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_TRIANGLES,0,va.GetCount()));
}