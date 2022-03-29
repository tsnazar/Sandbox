#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);
	~Shader();

	void Bind() const;
	void Unbind() const;
	
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, const glm::vec4& value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	std::string ParseShader(const char* filepath) const;
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile);
	
	void CheckCompileErrors(unsigned int object, std::string type);

	unsigned int GetUniformLocation(const std::string& name);
};
