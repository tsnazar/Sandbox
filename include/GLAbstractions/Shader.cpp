#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glad/glad.h>

#include "Core.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
	:m_RendererID(0)
{
	m_RendererID = CreateShaderProgram(vertexFile, fragmentFile, geometryFile);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

std::string Shader::ParseShader(const char* filepath) const
{
	std::ifstream stream(filepath);
	std::stringstream ss;
	
	ss << stream.rdbuf();
	stream.close();

	return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	
	if(type == GL_VERTEX_SHADER)
		CheckCompileErrors(id, "VERTEX");
	else if(type == GL_FRAGMENT_SHADER)
		CheckCompileErrors(id, "FRAGMENT");
	else 
		CheckCompileErrors(id, "GEOMETRY");
	
	return id;
}

unsigned int Shader::CreateShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile)
{
	GLCall(unsigned int program = glCreateProgram());

	std::string vertexCode = ParseShader(vertexShaderFile);
	std::string fragmentCode = ParseShader(fragmentShaderFile);
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexCode);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

	std::string geometryCode;
	unsigned int gs;
	if (geometryShaderFile != nullptr)
	{
		geometryCode = ParseShader(geometryShaderFile);
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryCode);
	}
	
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	if (geometryShaderFile != nullptr)
	{
		GLCall(glAttachShader(program, gs));
	}

	GLCall(glLinkProgram(program));
	CheckCompileErrors(program, "PROGRAM");
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	if (geometryShaderFile != nullptr)
	{
		GLCall(glDeleteShader(gs));
	}
	return program;
}

void Shader::CheckCompileErrors(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		GLCall(glGetShaderiv(object, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(object, 1024, NULL, infoLog));
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << '\n'
				<< infoLog << "\n----------------------------------------------------- --" << std::endl;
		}
	}
	else
	{
		GLCall(glGetProgramiv(object, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLCall(glGetProgramInfoLog(object, 1024, NULL, infoLog));
			std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << '\n'
				<< infoLog << "\n----------------------------------------------------- --" << std::endl;
		}
	}
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float value) const
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}
void Shader::SetUniform1i(const std::string& name, int value) const
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}
void Shader::SetUniform2f(const std::string& name, float v0, float v1) const
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}
void Shader::SetUniform2f(const std::string& name, const glm::vec2& value) const
{
	GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) const
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
void Shader::SetUniform3f(const std::string& name, const glm::vec3& value) const
{
	GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniform4f(const std::string& name, const glm::vec4& value) const
{
	GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0])) ;
}

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
	auto locationSearch = m_UniformLocationCache.find(name);
	if (locationSearch != m_UniformLocationCache.end())
		return locationSearch->second;

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform " << name << " does\'nt exist" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}
