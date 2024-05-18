#include "Shader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& singlefile)
	: m_RendererID(0)
{
	ShaderProgramSource source = parse_shader(singlefile);
	m_RendererID = create_shader(source.vertexSource, source.fragmentSource);
}

Shader::Shader(const std::string& vertex_file, const std::string& fragment_file)
	: m_RendererID(0)
{
	ShaderProgramSource source = parse_shader(vertex_file, fragment_file);
	m_RendererID = create_shader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::parse_shader(const std::string& singlefile)
{
	std::ifstream stream(singlefile);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	std::string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

std::string fileread(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::stringstream ss;
	std::string line;
	while (getline(stream, line))
		ss << line << "\n";
	return ss.str();
}

ShaderProgramSource Shader::parse_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	return { fileread(vertex_shader), fileread(fragment_shader) };
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)malloc(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		free(message);
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::create_shader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::set_uniform_1i(const std::string& name, int value)
{
	GLCall(glUniform1i(get_uniform_location(name), value));
}

void Shader::set_uniform_1f(const std::string& name, float value)
{
	GLCall(glUniform1f(get_uniform_location(name), value));
}

void Shader::set_uniform_1d(const std::string& name, double value)
{
	GLCall(glUniform1d(get_uniform_location(name), value));
}

void Shader::set_uniform_2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(get_uniform_location(name), v0, v1));
}

void Shader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

void Shader::set_uniform_mat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::get_uniform_location(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}
