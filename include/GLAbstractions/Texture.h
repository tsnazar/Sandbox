#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& path);

	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;


	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
private:
	void LoadFromFile(const std::string& path);
	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
};