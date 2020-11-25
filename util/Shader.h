#ifndef SHADER
#define SHADER

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	// caching for uniforms
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string &filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniform
	void setUniform1i(const std::string &name, int i);
	void setUniform1f(const std::string &name, float f);
	void setUniform2f(const std::string &name, float f1, float f2);
	void setUniform2f(const std::string &name, glm::vec2 f);
	void setUniform4f(const std::string &name, float f1, float f2, float f3, float f4);
	void setUniformMat4f(const std::string &name, const glm::mat4 &matrix);

private:
	ShaderProgramSource ParseShader(const std::string &filePath);
	unsigned int CompileShader(unsigned int type, const std::string &source);
	unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

	int GetUniformLocation(const std::string &name) const;
};

#endif