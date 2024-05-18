#pragma once

#include "gl/glew.h"

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
public:
	Shader(const std::string& singlefile);
	Shader(const std::string& vertex_file, const std::string& fragment_file);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void set_uniform_1i(const std::string& name, int value);
	void set_uniform_1f(const std::string& name, float value);
	void set_uniform_1d(const std::string& name, double value);
	void set_uniform_2f(const std::string& name, float v0, float v1);
	void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
	void set_uniform_mat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource parse_shader(const std::string& singlefile);
	ShaderProgramSource parse_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int compile_shader(unsigned int type, const std::string& source);
	unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int get_uniform_location(const std::string& name) const;
};
