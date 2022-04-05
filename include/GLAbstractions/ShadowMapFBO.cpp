#include "ShadowMapFBO.h"
#include "Core.h"

static GLenum ShadowMapTypeToGLType(ShadowMapType type)
{
	return type == ShadowMapType::TEXTURE2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
}

static void BindTexture(ShadowMapType type, unsigned int ID)
{
	GLCall(glBindTexture(ShadowMapTypeToGLType(type), ID));
}

static void AttachDepthTexture(ShadowMapType type, unsigned int width, unsigned int height, unsigned int ID)
{
	switch (type)
	{
	case ShadowMapType::TEXTURE2D:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		break;
	case ShadowMapType::CUBEMAP:
		for (unsigned int i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		}
		break;
	}

	GLenum textureType = ShadowMapTypeToGLType(type);
	GLCall(glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ID, 0));
}

ShadowMapFBO::ShadowMapFBO(ShadowMapType type, unsigned int width, unsigned int height)
	:m_Type(type),m_Width(width), m_Height(height)
{

	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glGenTextures(1, &m_ShadowMap));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	BindTexture(m_Type, m_ShadowMap);
	AttachDepthTexture(m_Type, m_Width, m_Height, m_ShadowMap);

	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glDrawBuffer(GL_NONE));

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		SANDBOX_ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}


ShadowMapFBO::~ShadowMapFBO()
{
	GLCall(glDeleteTextures(1, &m_ShadowMap));
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void ShadowMapFBO::Bind() const
{
	GLCall(glViewport(0, 0, m_Width, m_Height));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void ShadowMapFBO::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMapFBO::BindDepthTexture(unsigned int slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(ShadowMapTypeToGLType(m_Type), m_ShadowMap));
}
