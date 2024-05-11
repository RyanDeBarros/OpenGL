#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	ShaderProgramSource parse_shader();
	unsigned int compile_shader(unsigned int type, const std::string& source);
	unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int get_uniform_location(const std::string& name);
};