#pragma once
#include <glad/glad.h>

enum class ShadowMapType
{
	None = 0,
	TEXTURE2D, CUBEMAP,
};

class ShadowMapFBO
{
public:
	ShadowMapFBO(ShadowMapType type, unsigned int width, unsigned int height);

	~ShadowMapFBO();

	void Bind() const;
	void Unbind() const;

	void BindDepthTexture(unsigned int slot = 0);

	unsigned int GetTextureID() { return m_ShadowMap; }

private:
	ShadowMapType m_Type;
	unsigned int m_Width, m_Height;
	unsigned int m_RendererID = 0;
	unsigned int m_ShadowMap = 0;
};